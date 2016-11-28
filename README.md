UWB-Adpater
========
[![GitHub version](https://img.shields.io/badge/version-v1.4-brightgreen.svg)](https://github.com/KitSprout/UWB-Adapter)
[![GitHub old version](https://img.shields.io/badge/old%20version-%20v1.0-green.svg)](https://github.com/KitSprout/UWB-Adapter/releases/tag/v1.0)
[![GitHub license](https://img.shields.io/badge/license-%20MIT%20%2F%20CC%20BY--SA%204.0-blue.svg)](https://github.com/KitSprout/UWB-Adapter/blob/master/LICENSE)
[![GitHub pcb library](https://img.shields.io/badge/pcb%20library-%20v2.13-yellow.svg)](https://github.com/KitSprout/AltiumDesigner_PcbLibrary/releases/tag/v2.13)


UWB Adapter 是一款主要是以電腦開發為主的 UWB 室內定位模組，算是一個簡化版的 [KDWM1000](https://github.com/KitSprout/KDWM1000)，將感測器去掉，並結合具 VCP 功能的 JLINK 燒錄器，隨身碟的外型，只需要插入 USB 孔，就可以燒錄程式，透過 UART 與控制器通訊。

Hardware
========
* 控制器　 : [STM32F411CE](http://www.st.com/web/en/catalog/mmc/FM141/SC1169/SS1577/LN1877/PF260148) 48Pin 100MHz DSP FPU
* UWB 　　 : DecaWave [DWM1000](http://www.decawave.com/products/dwm1000-module)
* PCB 尺寸 : 18 x 45 mm
* 設計軟體 [Altium Designer 16](http://www.altium.com/en/products/altium-designer) ( PcbLib use AD [PcbLib v2.13](https://github.com/KitSprout/AltiumDesigner_PcbLibrary/releases/tag/v2.13) )

View
========
<img src="https://lh3.googleusercontent.com/nTiRdlZaBRoMVYdkjJkaXsTM8M-4QYJKMCwBH1-y6ej4Lb7k0P6pflj7u1wLLePUFQlK6DKydyP0coxKSJ6YALfjY_UHUSg0WMATQ9vTMFQmshgJMDcm3jAmbV0D9y7D8aawAx0WvKi21L0GkEgGNeBhFe5-tr33cb504EhPiKRMvdt6SWf3p8K2RdAEYTy5VEPp7vLE4GijMLNgv4nPXIKhAT_OkYsavOOE1pymX-7TSXN9tXNUBbtUoSTrDqZEL8iD5_g2ckY11lcUN6TT4_SSykPdyeM6Y9i9J5wBtqjuv4qWwPAyIHucKg-8uGkUZ9PNxPwUbSQU1JXuM4_ruvIAMv6SbITvsRkaB-AxYLNzPltPUVl1aM_Ld6gTA_owORfRdYHlypMbrkwwVZiVF8sO9U03R7NFsNPSc6X44VXWIBWHwMJcNZny50boPub157o2CI3u2qCt-Jl4ZwPTlubrJnu4fuvOzpI7tRqMnkFJ0dv13Wct7Cuk4EyLCyZw9Efxcw_viriV3XrQmJ6kEPuYkLLaRw2Diw9EBtAGzJ-ad0q995miV31UJVkh-jeeiPLBuk04NnQHDePkpp9e_WfLw47YuVkDJ18r0MtgC-D88prn=w1027-h770-no"/>
<img src="https://lh3.googleusercontent.com/gUClZY9-XFiZ5tJplYFhKhRbA0_nXFLyqR83ZTpQExrSW1BCio3zSl_jv_yaw_1kMSIev0uAdebZ5niTjcfgywy3UJRFdrlqxn6jZrE_KgxTyXUTF6neJGV2tX-AJfQUsFck_w7T1HV-c-PtzAt_n5goj59GQJfiWhmMSppJSF_aBVB2ycDGo_gVCJfhWQRDP-TP15UpHEni2g3yILjf9ttmWPKlMiXIKdvrSVA0vRx9cTB9SySe6kw8S1mdY43LV6BJjCLIvl2mlAx8vv0FYwsSt8VbajlctJ7rVOdXiBTtHgrpp55kd3roxhjaCs_AkU3BqbZHYOwS026L4JsT1aCIZ8eu81ztf9f_Av9uHWuCktOa1REmX6s334vK1oRctTbn10XZwVMGpdz3UwO-p8lnuENbWDXsRuizdqDPuDixiZRM9J-8qrg9LkLA3qPzvD7mRnKAX-DBr67AKWCzI1t4THGaHSsbFo2KMqJpyMEd-GkoNG9Vr2aLTa_nLMmDuJjnyN-IxJGu7GGr7ULvNqQQX_qbZO0bSlFuxoWrmneI7bBC1_AatRH3kP8292d5p0PhniASbWiP1fgsisPL-uOf03GHi37L8puWAibbZ_mAlk8C=w1210-h770-no"/>
<br />
[more photo...](https://goo.gl/photos/W93huj9bQzaHpJsv7)

Schematic
========
<img src="https://lh3.googleusercontent.com/c4mD40uMLFW5CqqyEBMwfKTQPCN77kW-q13_ayM0_zCC94S1zHUOHmuw6dSzhiFDUUgwLsDnWk2rqTI7NdUDC506RvfhuQIEVL9MiXpB_e6nqg6vIbP0iTCSMxUTShftcjg3CLdLAXXV7SG8b-oRoKVZeUf99lTk7NIFFYqT1eO-Mcmw-cPz0p3RUHTg9kaBU8EySr4d6BSIdAjSH_DqUUvebV6AeQl0bfi-FRLDKwc1lb90NRTwFT5SnmGZV0T4UIFf-S4lavX4P3Q_XYfFsDMo-MyWH3-0T22eiueW1i3-oD4HBgjYNHcCBlYcUUovLmycg6-B0g4FeLCeLwokPZdsJVBt2NFyDNMxfxNdnQdJWCIwWZS0KHRe29Gc8nVT8p6AFhhCXBmE4ASl3kgCdLvyRZNMAf-OpksnnodeDozSdmR_s9lP50vLqXvn7lf5NkqsxM9oenq5U3-BibINQ_VAaT_0Hwuo-DcJMgChJj3ERsiKxr93IvUkTTCaWUhyNYDDepUm0NAANO9F4bu4-oGgKA7BCruGoTcLxdMR5BFiHN4xeWPSaNedUZdBOtKLXTx0PugNJfxbMhAf0bo5yFczbB4AaKF9ts081ZkdyE5vGczT=w1156-h770-no"/>
