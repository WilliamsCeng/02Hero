# 02Hero
2020年新工科联盟-Xilinx暑期学校（Summer School）项目

├─hls    HLS相关文件
│      aes.cpp
│      aes.h
│      aes_test.cpp
│      directives.tcl
│      
├─hw   vivado搭建硬件平台的相关文件
│  └─bitstream
│          AES_En_De.bit
│          AES_En_De.hwh
│          AES_En_De.tcl
│          
└─pynq   放到PYNQ-Z2的应用层文件
    └─AES_En_De_PYNQ
        │  AES_En_De_final_demo.ipynb
        │  sds_trace_data.dat
        │  
        ├─bitstream
        │      AES_En_De.bit
        │      AES_En_De.hwh
        │      AES_En_De.tcl
        │      readme.md
        │      
        ├─files   功能测试：文本文件加解密  
        │      decrypted_encrypted_test.md
        │      encrypted_test.md
        │      test.md
        │      
        └─images   功能测试：图片加解密
            ├─lena
            │      decryption_image.jpg
            │      encryption_image.jpg
            │      lena.jpg
            │      original_image.jpg
            │      
            ├─paris
            │      decryption_image.jpg
            │      encryption_image.jpg
            │      original_image.jpg
            │      paris.jpg
            │      
            └─toys
                    decryption_image.jpg
                    encryption_image.jpg
                    original_image.jpg
                    toys.jpg
                    