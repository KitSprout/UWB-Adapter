[UWB-Adpater](https://github.com/KitSprout/UWB-Adapter)
========
* Author  : [Hom](http://about.me/Hom)
* Version : v1.0
* Update  : 2016/09/03

Description
========
UWB Adapter 是一款主要是以電腦開發為主的 UWB 室內定位模組，算是一個簡化版的 [KDWM1000](https://github.com/KitSprout/KDWM1000)，將感測器去掉，並結合具 VCP 功能的 JLINK 燒錄器，隨身碟的外型，只需要插入 USB 孔，就可以燒錄程式，透過 UART 與控制器通訊。

License
========
* 硬體(Hardware)採用 [CC BY-SA 4.0](http://creativecommons.org/licenses/by-sa/4.0/deed.zh_TW)  方式授權 
  
　　<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/deed.zh_TW"><img alt="創用 CC 授權條款" style="border-width:0" src="http://i.creativecommons.org/l/by-sa/3.0/tw/80x15.png" /></a>  
　　<span xmlns:dct="http://purl.org/dc/terms/" property="dct:title"> UWB-Adpater v1.0 </span>由<a xmlns:cc="http://creativecommons.org/ns#" href="http://about.me/Hom" property="cc:attributionName" rel="cc:attributionURL"> Hom </a>製作，以<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/deed.zh_TW"> 創用CC 姓名標示-相同方式分享 4.0 國際 授權條款 </a>釋出。  

* 軟體(Software)採用 [MIT License](http://opensource.org/licenses/MIT) 方式授權  

Hardware
========
* 控制器　 : [STM32F411C](http://www.st.com/web/en/catalog/mmc/FM141/SC1169/SS1577/LN1877/PF260148) 48Pin 100MHz DSP FPU
* UWB 　　 : DecaWave [DWM1000](http://www.decawave.com/products/dwm1000-module)
* PCB 尺寸 : 20 x 45 mm
* 設計軟體 [Altium Designer 16](http://www.altium.com/en/products/altium-designer) ( PcbLib use AD [PcbLib v2.8](https://github.com/KitSprout/AltiumDesigner_PcbLibrary/releases/tag/v2.5) )

Software
========


Related Documents
========
* [Google Drive](https://goo.gl/BwtKp3)

View
========
<img src="https://lh3.googleusercontent.com/xrH78t7kvFd-e9IX7N9VEfGFNt2klRsUjnuCyqIAXT35qY0aGjk0AVuAtafno5eKB3DAiXcPcEfD8omTFgalIThtrMW8DvKvj5_-Sy7swP1jdje9u2U3MP-yLZWwNQtiUnh2mBddp8SlC_KBVHVrrZWH-lNdSb-_8Ccb-IvxtH1dlb8jGjF7JaYKM6aKF-Jwozhx85XXeA-SQN0a6hgHDXZ3R63xx9KH7zOesTrFm8-dAEQVR9mpiorMxOctj3-_OSZUvv5r4M-sAWt3tfPj99tnz3e3wxuCsO1L14M9DNyVVTnTJXxEVDDPJgb_ivfsHtriRf6P9FjhhmlUyfAJXeuj45YMzSUxyHh_AlBqKRhJgzfCsFdNPAnaiM31hfmye_h2a8uwG2geFLlAQWFn6sviKLVvUCTXoFUUWiP_UciT9KV99VIMyD01OjlR15GQ0HzO0zkwkGhYnwnzJQIeM6CbSJPIOL5OB1U12sVVCj31ko6NFE6ncvVn9z6gLtuOF7CZ6DqTEawq0wqOHsivejMfnYt5sKDwTzDsU1hDKirFCK3zEqW7E_ua_nxu5og4B6pjOKKJYNS8yHibtvj9eIS330Gcm7WwTt2M-kcmca39tJb5WA=w1057-h775-no"/>

<br />
[more photo...](https://goo.gl/photos/ZdrpX1kvVhHjHhLu7)

Schematic
========
<img src="https://lh3.googleusercontent.com/KhxybH99FP8IcjEvv_sCG0FTx9luJRgMkjn7dnYSP7kPLICY0fil9jPJ5gf8aNyzvuK4hrXqDjbXVlH3eCkBXAfM3VAfzWMvyGPzrS4zmX4QRMPh65JXBv1lK3s-zYyeDeAtds60qHTEHyg_rHCZXeckx0NS3vb8t5z1o5L3PXqr4ddQQZqSPnow9QoWN_JIVj2CICrdaDqZTVLc6DRpLJetXZNYv7wvQt6LsVZVkU4EtJHDY5HDF8BUhOfTMan64Hwe9DCV6W-xO0C6CRQlGekc5fuy8GKxjAzrFCOltXQOSiSevFw189ziEguoSpW-fxBgY4-6tG1nF3GIqhu71YFekXsrEYKwUaigFOgh3j-u8mw-lijad45blS4MtbGZ_ECFkoMWyiof6OQkWCvi4brm3QTYlgfuEFgroa7NzZO0ZoJ54LgL5xOHOWqzay2NZW6IdF7phzdZdYdDrPanvrV1NdamqUkYGGyQc2AqaLS8gA29jER4gFEl7JVzFptsW26dyg2zJXqa4gOkuZx8X-hlT5O4iDCQpGNkj2H6KqLhtyyPFQh0TkKLubNsMC4BGqGloXR8cioDtZ3JWubWypccBuOGfEycObfriBizV9g3wTM6yg=w1292-h775-no"/>
