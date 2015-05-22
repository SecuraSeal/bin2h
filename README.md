bin2h
=====

Console utility to convert binary (.bin) to C header file.


### Usage:
```
./bin2h FILE -f STRING -b STRING
./bin2h FILE -b STRING
./bin2h FILE -f STRING
```


### Commands:
```
-v, --version          output version information and exit
-h, --help             display this help and exit
-f, --file             [OPTIONAL] - specify location and name of header file
-b, --blobname         [OPTIONAL] - use this name as binary blob name
```

### Output:
```
#ifndef __blobname_h__
#define __blobname_h__

static FW_SECTION const unsigned char blobname[] = 
{
};

#endif /* __blobname_h__ */
```


### Update:
If main.c is edited, you need to run make_bin2h to generate a new bin2h:
```
./make_bin2h
```
