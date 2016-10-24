[UWB-Adpater](https://github.com/KitSprout/UWB-Adapter)
========
* Author  : [Hom](http://about.me/Hom)
* Version : v1.2
* Update  : 2016/10/24

Description
========
UWB Adapter 是一款主要是以電腦開發為主的 UWB 室內定位模組，算是一個簡化版的 [KDWM1000](https://github.com/KitSprout/KDWM1000)，將感測器去掉，並結合具 VCP 功能的 JLINK 燒錄器，隨身碟的外型，只需要插入 USB 孔，就可以燒錄程式，透過 UART 與控制器通訊。

License
========
* 硬體(Hardware)採用 [CC BY-SA 4.0](http://creativecommons.org/licenses/by-sa/4.0/deed.zh_TW)  方式授權 
  
　　<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/deed.zh_TW"><img alt="創用 CC 授權條款" style="border-width:0" src="http://i.creativecommons.org/l/by-sa/3.0/tw/80x15.png" /></a>  
　　<span xmlns:dct="http://purl.org/dc/terms/" property="dct:title"> UWB-Adpater v1.2 </span>由<a xmlns:cc="http://creativecommons.org/ns#" href="http://about.me/Hom" property="cc:attributionName" rel="cc:attributionURL"> Hom </a>製作，以<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/deed.zh_TW"> 創用CC 姓名標示-相同方式分享 4.0 國際 授權條款 </a>釋出。  

* 軟體(Software)採用 [MIT License](http://opensource.org/licenses/MIT) 方式授權  

Hardware
========
* 控制器　 : [STM32F411CE](http://www.st.com/web/en/catalog/mmc/FM141/SC1169/SS1577/LN1877/PF260148) 48Pin 100MHz DSP FPU
* UWB 　　 : DecaWave [DWM1000](http://www.decawave.com/products/dwm1000-module)
* PCB 尺寸 : 18 x 45 mm
* 設計軟體 [Altium Designer 16](http://www.altium.com/en/products/altium-designer) ( PcbLib use AD [PcbLib v2.9](https://github.com/KitSprout/AltiumDesigner_PcbLibrary/releases/tag/v2.9) )

Related Documents
========
* [Google Drive](https://goo.gl/BwtKp3)

View
========
<img src="https://lh3.googleusercontent.com/fk2Kc9odZnPG5ZeOJ6eZNhcj91HX9Rdyk078w6wL3rfm9mlew1GjFBH6mgCDM-Ib-ilwncA5m2rcO-9EiJsEZAA4aGt3liCj0l-JwmDymD-1LtKpG3JcCMYZof0KEep4byqZ91EISE7IFPickm6-M8CVQBjGmOK_klZSlhKVoUPCkmFb4wl0omlvsg2U12XuhoigBFnf_DeVVJyZMPTXLYPios7s7fhfpkmzL2znr6fLPhfobdv8S6DtvkSHvwDwntvDSuVJWLgHaxqSGJdW-aZOSOl1Ugg1Pk3BVGK8gYyF4rmMaxgkAOg0bFz7nLQA8iDKkfxlyEM57CfRZtClPn-VAFNq2pKK8xXCIX9s-jXJvJ_lStqEUbmERKiU4v6brxP6_e9D9Ja_Cfqiv6e13YX-5YQ3d3n8DhNkwlCMxg8ok4JVJyToOa21nsCulMVUB_HV60tb8HsnWb9K-8Q4QECD5eSLjgR2uR7P2RAtkZ6lWedDP0lDzDQnbcwetrRXFv98POnxGlzbDShPuoJKwhEGRy4hw8zTffX_Zd9qLUMDHIc6XGJm8qnAcs_SlAfK9aU1H8NoSsZKbZyTnvQFKm-ERX9-3hjBWr9Iwb4mbEBd83pu=w1027-h770-no"/>
<img src="https://lh3.googleusercontent.com/s-TP3LNFN2ce6tsQohVsCh5CHz1Mj-ux-X-71d0hqgLHZDZifz0vshtw4ve0HO5wCff2Mzwj7EM5v0jBRoRMEEND8hxdXVT3lRSfEdGMzK0kZ9kS1TtAb-fgXCu_5cpH8vKxwEB406c-QU5W4gmEFfoJ9XRNpYMp81rT6YBmX7lRmNiJNJhVM9iH5LFvYUVo-RGHWJegiQo7S74w5M8b7Iqt9kcWpkjWalIkk3ZzNXkTNpQadr8W_E5rnuDfdELm3glFO_O4coJFF0avwyeESf3xsKzzCZ9qY8tuitbml7T9EJHrAyA_qV-9-LDij_g-fo-Rbj_KdK4lbhB4zEV80asNGpVvKSXj0VIgebkvxw_7FFEClpRgv8jSgCkandR1yebtDhXD8syhELVfL95RAqxK5TN-jKQNTGcTxXQeQupW8TF2JTE29zx4h5eVhd8cdxJsX4CK6t80J_EFjErKcZXu-PX4zxbSubDRBqIS472d9C0VTbMgOpJkAJ2rC5uwmeRRMg8YPlqviojl1C5yQCNatMX3kU1klN8Pnjkj2jHmYy3o9x_N4_pqDgGPyDs0TqnNh7FvfeqfNegQv1JNBRBGx6ZqbGz3dhpx1e0dA2mOAQvC=w1146-h770-no"/>
<br />
[more photo...](https://goo.gl/photos/W93huj9bQzaHpJsv7)

Schematic
========
<img src="https://lh3.googleusercontent.com/Wc08Ktbk6MUL2W9X8YwCiS4WFa_gi31MC_1IWu6o_CF-cFNs8evkTojFijbeqiLg-1qPumgdc2H9h-hRS_HXvAtb43-zfLJUNn2CmL8XdrWtvRYEqFtN2M89HXvSE6MPmH63u5rPEmG6e01JR4dBvx3XdJbavF39yQJ8P_Ch190HrWiAKaE5gwMaG0EWJ35CNjr8QCq4Qje-4NHH3JQtrLNDCA3MEWx2BgB3BNdXFEDZPn6c-lXTSRcLbIJGynmLxLgQC-JqnPWFHJb3tpRz11s60OU099xKf9pFmQ3tXbYZeb9E8hSacF5t-kFLJzC8C1pTSmVIXNYX2sOOL_UD1eQL3uK66zES_BaXkW1aaKTqnS-WEZWy-iX3gQ4I6E3o1cVk1J8w8-nXk9-O1usiaGsSHRoYq8PAD99B8wBT9lc86PRgBt_yTKojEV52Aq2n4HFqg8A566U3km3KiO4mZAys1CWWF_lYYNgD5hOtjJSIIWumpyLAajaONYM_2awz3N39UKCLwzlkvWUieKL1YOhYX3yU1Jail3YThg6wHRTb_uecl8pOcTGdgPt-w27HIjM7Pdsz_wmudeH-77q1EZw9hT2Iau7R2Qpd5NZTwngLA7g-=w1155-h770-no"/>
