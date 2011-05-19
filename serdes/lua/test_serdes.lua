require("serdes");
ORD_NONE = 2;
ORD_BE = 1;
ORD_LE = 0;

SZ_BYTE = 8;
SZ_LONG  = SZ_BYTE*4;
SZ_SHORT  = SZ_BYTE*2;
SZ_CHAR  = SZ_BYTE*1;

FP_IEEE754 = 0;
FP_IBM  = 1;
FP_CRAY = 2;
FP_VAX  = 3;

sd_push = serdes.new();

for i=20,32,1 do
    sd_push:push_uint(0x12345, i, ORD_NONE, ORD_BE);
end
for i=20,32,1 do
    sd_push:push_uint(0x12345, i, ORD_NONE, ORD_LE);
end

sd_push:push_sint(-2, 13, ORD_NONE, ORD_LE);

sd_push:push_uint(0x12345678, SZ_LONG, ORD_LE, ORD_BE);
sd_push:push_uint(0x12345678, SZ_LONG, ORD_BE, ORD_BE);
sd_push:push_uint(0x12345678, SZ_LONG, ORD_LE, ORD_LE);
sd_push:push_uint(0x12345678, SZ_LONG, ORD_BE, ORD_LE);


sd_push:push_fp(3.14, SZ_LONG, FP_IEEE754, ORD_LE, ORD_BE);
sd_push:push_fp(3.14, SZ_LONG, FP_IEEE754, ORD_BE, ORD_BE);
sd_push:push_fp(3.14, SZ_LONG, FP_IEEE754, ORD_LE, ORD_LE);
sd_push:push_fp(3.14, SZ_LONG, FP_IEEE754, ORD_BE, ORD_LE);

sd_push:push_fp(3.14, SZ_LONG*2, FP_IEEE754, ORD_LE, ORD_BE);
sd_push:push_fp(3.14, SZ_LONG*2, FP_IEEE754, ORD_BE, ORD_BE);
sd_push:push_fp(3.14, SZ_LONG*2, FP_IEEE754, ORD_LE, ORD_LE);
sd_push:push_fp(3.14, SZ_LONG*2, FP_IEEE754, ORD_BE, ORD_LE);

print(sd_push:get());

sd_pull = serdes.new();
bs, pos = sd_push:get();
sd_pull:set(bs, 1);

for i=20,32,1 do
    print(string.format("0x%.8x",sd_pull:pull_uint(i, ORD_NONE, ORD_BE)));
end

for i=20,32,1 do
    print(string.format("0x%.8x",sd_pull:pull_uint(i, ORD_NONE, ORD_LE)));
end

print(string.format("0x%.8x",sd_pull:pull_sint(13, ORD_NONE, ORD_LE)));

print(string.format("0x%.8x",sd_pull:pull_uint(SZ_LONG, ORD_LE, ORD_BE)));
print(string.format("0x%.8x",sd_pull:pull_uint(SZ_LONG, ORD_BE, ORD_BE)));
print(string.format("0x%.8x",sd_pull:pull_uint(SZ_LONG, ORD_LE, ORD_LE)));
print(string.format("0x%.8x",sd_pull:pull_uint(SZ_LONG, ORD_BE, ORD_LE)));

print(sd_pull:pull_fp(SZ_LONG, FP_IEEE754, ORD_LE, ORD_BE));
print(sd_pull:pull_fp(SZ_LONG, FP_IEEE754, ORD_BE, ORD_BE));
print(sd_pull:pull_fp(SZ_LONG, FP_IEEE754, ORD_LE, ORD_LE));
print(sd_pull:pull_fp(SZ_LONG, FP_IEEE754, ORD_BE, ORD_LE));

print(sd_pull:pull_fp(SZ_LONG*2, FP_IEEE754, ORD_LE, ORD_BE));
print(sd_pull:pull_fp(SZ_LONG*2, FP_IEEE754, ORD_BE, ORD_BE));
print(sd_pull:pull_fp(SZ_LONG*2, FP_IEEE754, ORD_LE, ORD_LE));
print(sd_pull:pull_fp(SZ_LONG*2, FP_IEEE754, ORD_BE, ORD_LE));
