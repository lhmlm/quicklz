# QuickLZ概述

QuickLZ号称是世界上速度最快的压缩库，速度可达到单核308 Mbyte/s，使用简单，易于集成。<br>
有关quicklz的更多信息，请参阅[http://www.quicklz.com](http://www.quicklz.com) 。

# 示例介绍

- 配置压缩等级选项，配置为level 1（有三种等级 1/2/3，level 1 压缩速度最快，压缩比最小，level 3 压缩速度最快，压缩比最大）;

# 示例代码
## 字符串的压缩、解压缩
``` c
#include "quicklz.h"

#if QLZ_STREAMING_BUFFER > 0
    #error Define QLZ_STREAMING_BUFFER to 0 for this demo
#endif

int main ()
{
    qlz_state_compress *state_compress = (qlz_state_compress *)malloc(sizeof(qlz_state_compress));
    qlz_state_decompress *state_decompress = (qlz_state_decompress *)malloc(sizeof(qlz_state_decompress));
    char original[] = "LZ compression is based on finding repeated strings: Five, six, seven, eight, nine, fifteen, sixteen, seventeen, fifteen, sixteen, seventeen.";

    // Always allocate size + 400 bytes for the destination buffer when compressing.
    char *compressed = (char *)malloc(strlen(original) + 400);
    char *decompressed = (char *)malloc(strlen(original));
    int r;

    r = qlz_compress(original, compressed, strlen(original), state_compress);
    printf("Compressed %d bytes into %d bytes.\n", strlen(original), r);

    r = qlz_decompress(compressed, decompressed, state_decompress);
    printf("Decompressed back to %d bytes.\n", r);

    return 0;
}
```

## 流模式的压缩、解压缩
``` c
#include "quicklz.h"

#if QLZ_STREAMING_BUFFER == 0
    #error Define QLZ_STREAMING_BUFFER to a non-zero value for this demo
#endif

int main ()
{
    qlz_state_compress *state_compress = (qlz_state_compress *)malloc(sizeof(qlz_state_compress));
    qlz_state_decompress *state_decompress = (qlz_state_decompress *)malloc(sizeof(qlz_state_decompress));

    // Allocate data buffers. 200 and 200 + 400 bytes should be sufficient for our test data packets.
    char *compressed = (char *)malloc(200 + 400);
    char *decompressed = (char *)malloc(200);

    int o = 0;

    // Zero out both states.
    memset(state_compress, 0, sizeof(state_compress)); 
    memset(state_decompress, 0, sizeof(state_decompress)); 

    // Always decompress data in the same order as it was compressed. There is no requirement on destination address when decompressing
    // even though we do it consecutively in this example using the o pointer.
    qlz_compress("This is data packet number one, ", compressed, 32, state_compress);
    o += qlz_decompress(compressed, decompressed, state_decompress);

    qlz_compress("this is data packet number two, ", compressed, 32, state_compress);
    o += qlz_decompress(compressed, decompressed + o, state_decompress);

    qlz_compress("and finally data packet number three.", compressed, 37, state_compress);
    o += qlz_decompress(compressed, decompressed + o, state_decompress);

    // printf() needs 0-termination.
    decompressed[o] = 0;
    printf("Concatenated decompressed results: %s", decompressed);

    return 0;
}
```

# 参考资料

- QuickLZ官方手册：[manual](http://www.quicklz.com/manual.html)


