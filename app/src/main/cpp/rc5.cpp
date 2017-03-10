#include <stdint.h>

#define w 16
#define r 12
#define b 16
#define t 26
#define c 8

void RC5_generateChildKey(uint8_t *KeyK,uint16_t *ChildKeyS);
void RC5_Encipher(uint16_t *In, uint16_t *Out);
void RC5_Decipher(uint16_t *In, uint16_t *Out);

uint16_t ChildKeyS[26]; // 2*r+2
uint8_t KeyK[16]={0x03, 0x1F, 0x18, 0x98, 0x5D, 0x2B, 0x44, 0x42, 0x86, 0x04, 0xA3, 0x55,
                  0x76, 0x89, 0xE0, 0xA4}; // 种子
/**
* 循环移位函数
* 由于在生成子密钥，加密，解密过程中都要进行循环移位，故要首先定义循环以为函数。
* 循环左移和右移函数
* x : 被循环的数
* y : 将要循环的位数
*/
#define ROTL(x,y) ((((uint16_t)(x))<<(y&(w-1)))|(((uint16_t)(x))>>(w-(y&(w-1)))))
#define ROTR(x,y) ((((uint16_t)(x))>>(y&(w-1)))|(((uint16_t)(x))<<(w-(y&(w-1)))))

/** 初始密钥产生函数 */
void EnDeCryptInit(void)
{
    RC5_generateChildKey(KeyK, ChildKeyS);
}
/**4、 密钥扩展函数
由于需加密 r 轮，每轮需要两个子密钥，所以需要密钥扩展函数根据初始密钥来扩展出 2r+2
个子密钥。 */
void RC5_generateChildKey(uint8_t* keyK, uint16_t* childKeyS)
{
//const double e = 2.718281828459;
//const double Phia = 1.618033988749;
    int PW = 47073;//0xb7e1;
    int QW = 40503;//0x9e37;//genggai
    int i;
    int u;
    uint16_t A,B,X,Y;
    uint16_t L[c];
//初始化数组 S
    childKeyS[0]=PW;
    for (i=1;i<t;i++)
    {
        childKeyS[i]=(childKeyS[i-1]+ QW);
    }
//将 K 数组转换为 L 数组
    for(i=0;i<c;i++)//初始化 L 数组 c=8
    {
        L[i]=0;
    }
    u = w/8;
    for (i=b-1;i!=-1; i--)//转换，数组 L 每一元素长为 32bit，数组 K 每一元素长为 8bit
    {
        L[i/u] = (L[i/u]<<8)+keyK[i];
    }

    //产生子密钥，存储在 ChildKeyS 中
    A=B=X=Y=0;
    for(i=0;i<3*t;i++)
    {
        X = childKeyS[A] = ROTL(childKeyS[A]+X+Y,3);
        A = (A+1) % t;
        Y = L[B] = ROTL(L[B]+X+Y,(X+Y));
        B = (B+1) % c;
    }
}
void RC5_Encipher(uint16_t * In, uint16_t * Out)
{
    uint16_t X,Y; //定义两个 32 位存储器
    int i;
    uint16_t *S = ChildKeyS;
    X = In[0]+S[0];
    Y = In[1]+S[1];
    for( i=1;i<=r;i++)
    {
        X=ROTL((X^Y),Y) + S[2*i]; //异或，循环移位，相加
        Y=ROTL((Y^X),X) + S[2*i+1];
    }
    Out[0]=X;
    Out[1]=Y; //密文
}
void RC5_Decipher(uint16_t* In, uint16_t* Out)
{
    int i=0;
    uint16_t X,Y;
    uint16_t *S = ChildKeyS;
    X = In[0];
    Y = In[1];
    for(i=r;i>0;i--)

    {
        Y = ROTR(Y-S[2*i+1],X)^X; //相减，循环移位，异或
        X = ROTR(X-S[2*i],Y)^Y;
    }
    Out[0]=X - S[0];
    Out[1]=Y - S[1]; //明文
}
// 解密函数
void RC5_Decrypt(uint8_t *src, uint8_t *dst, uint8_t len)
{
    uint8_t i;
    uint16_t _src[2];
    uint16_t _dst[2];
    for(i=0; i<len; i+=4)
    {
        _src[0] = src[i];
        _src[0] <<= 8;
        _src[0] += src[i + 1];
        _src[1] = src[i+2];
        _src[1] <<= 8;
        _src[1] += src[i + 3];
// decrypt data
        RC5_Decipher(_src, _dst);
        dst[i] = (uint8_t)((_dst[0] >> 8) & 0xFF);
        dst[i + 1] = (uint8_t)(_dst[0] & 0xFF);
        dst[i + 2] = (uint8_t)((_dst[1] >> 8) & 0xFF);
        dst[i + 3] = (uint8_t)(_dst[1] & 0xFF);
    }
}
// 加密函数
void RC5_Encrypt(uint8_t *src, uint8_t *dst, uint8_t len)
{
    uint8_t i;
    uint16_t _src[2];
    uint16_t _dst[2];
    for(i=0; i<len; i+=4)
    {
        _src[0] = src[i];
        _src[0] <<= 8;
        _src[0] += src[i + 1];
        _src[1] = src[i+2];
        _src[1] <<= 8;
        _src[1] += src[i + 3];
// decrypt data
        RC5_Encipher(_src, _dst);
        dst[i] = (uint8_t)((_dst[0] >> 8) & 0xFF);
        dst[i + 1] = (uint8_t)(_dst[0] & 0xFF);
        dst[i + 2] = (uint8_t)((_dst[1] >> 8) & 0xFF);
        dst[i + 3] = (uint8_t)(_dst[1] & 0xFF);
    }
}