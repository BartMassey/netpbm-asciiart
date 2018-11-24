# netpbm-asciiart
Copyright (c) 1990 Bart Massey

<table align="center" style="width: 70%">
<caption style="font-size: 70%; text-align: left; caption-side: bottom">
  <i>Left: </i>Sample 132-column <tt>mkasciiart</tt> output;
  <i>Right: </i>Result of recycling this ASCII art
     through <tt>asciiartopgm</tt></i>
</caption>
<tr>
<td><pre style="font-size:3pt;line-height:130%">
                                                          `                                                                         
                                                 ``````     ````````` ``````                                                        
                                            ```        `   `                  ````````````                                          
                                        ```     ` .'_!+YFhhyIf1\:,,,  `..,.  `              ```                                     
                                      ``   ` ,////(][F$Q88N#W8pVf((&lt;&lt;fFfJFfFFfl?(//;_,,,.     ```                                   
                                        `"&lt;Y{Y+\&lt;tk6MWW#8N6E8DPV}v?FI}FVGppAwIFkPZPwVYcL?&lt;&lt;!/'    ` ``                    `         
                                  ``  .(CIVC&lt;!!YUgN@MNpKDNg08#g68gAU8MgdZPZqQKPVVUKqGAm$%{fCCYc!:,,`   ``                           
                                    `:!&lt;?]/,/Lwpp##N0Q00O0EQp8@##8NNdE6##6UP3KAUPP$DQKZ$bE3VIywZkJ&lt;\",`  ``                         
                                 `._/&lt;lf{!:][FAmNBNmm888DU$pK3dN8N0D80aANNN0mdAO$KKP3qmKUdKGUZk3UPZwFt+,   ``                       
                             ` `'!l%FoUV+JKdVyU##@8gg#@BNpAQmmQN8N@g0&amp;gm0888#886m0mQpDm6mqpmAdpUGU3ZGUPI1('   `                     
                            ` -!FUmp0NKkb@@Q0}&amp;W@W#8@@WW@B0Kg&amp;N8#BW@N&amp;NNN#NgNg&amp;MMBBMN8#8BgqaDp0Ngd$UaA60DwF(,  `                    
                        `  ``_}pgN#8aDgaN@@#&amp;0@@@W#NW@NdKwIIw3$Vdg8W@MBB8NN#BNNNW@@WWW@B@B8EK0NNBgbUAEpNNgGwVL,  `                  
                       `   `,VNN#@#wkmlCQN@@#BN@##N8&amp;F\__._&lt;=CF?13UgN@W@@@NM@@@WWW@MM@@@W@@B88N88B@gpg0QNW#bZVy/   `                
                     `    ,,"6kN@Wg8U1,=Ig@@@86g88mmg1`  `:;/(L=(]FGD8@@@@@@WWW@@@@@@@@@@@W@@@B@BM@@@#MMBW@@8awZC:                  
                    ` `.  (&lt;!+0@BBgEV?:=VB@BgU8Ngk]1IEf,.,~`'!"&lt;!!f$Q8MNB@@@@@@@@@@@@@@@@@#M@@@@@@@W@@@@@@W@@WgK$w! `               
                      ,  ^EF,F@BNDPkF?+VB@@g1&lt;AMgt"!!!1F|/!\;!(:~\[UQN@NB@@@@@@@@@@@@@@@@@@BWW@@WW@@@@@@@@@@W@@WNQEY;`              
                  ` `: /"a8K!g@8P%F1!/+%a8@Ww/($gZ/.   ,/!/r&lt;!&lt;=_';(PEgNBWWWWW@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@W@@@W@N* `            
                 `  ,',$Pm&amp;m!bWmI}+/;::c\!3N@@QPkF(_ `  ````   ,/",:!FGm#W@@@WBWWWWWW@W@@@@@@@@@@@@@@@@@@@@@@@@@W@@W@0` `           
                ` `fF,;08g@U,Cgac+/,  ` '`,"!c1&lt;;.  ` ``         `.:_"\FkamNNW@@@@@@@@@@@W@@@@@@@@@@@@@WW@@@@@@@@@@@@@F `           
                 `Ff!,!GNm&amp;?'1Yr-  `    ` .              ````` ``    ``,"/!!+CIwZUGd0g#BW@@@@@@@@@@@@@@WW@@@@@@@@@@@W@N, `          
                  ;!!//f88Y/!.      ``` `     `   ```        `` ````      ''-,:,:"/!&lt;=J%ab#@@@@@@@@@@@@@@@@@@@@@@@@@@@B, `          
                `  ,`_'"0@F'  ```       `   -,,,....  ``   `` ``            ``` .',_;!!!&lt;JE@W@WW@@@@@@@@@@@@@@@@@@@@@@W: `          
                 `',/,,A@B"       `.,:/!*!/!!!!/;__:,':"/!*r&lt;!/;_'. `` `        ,':"/!(((&lt;IN@@W@@@@@@@@@@@@@@@@@@@@@W@k `           
                ` `?= FMN$::_/?[1}kZZUD0mAG$akyFf=r/"//+vYfFIFIFVPVFJv+!!^"_',`  ._!&lt;+&lt;++=kB@@@@@@@@@@@@@@@@@@@@@@@@W@3`            
               `  ,f&lt;!Fg@v `  '/[fFCIwkwww$QgNmVv(!""//?FD8#BBN88#80E3w%yyIFFC]c&lt;*!!+cc?+Fg@@@@@@@@@@@@@@@@@@@@@@@@W@N3! `          
              ` :&lt;JvF}a@@;   `!|!&lt;1%GmggpgNQg#@N%!' ` :~VW@@@B88g6ggmaw%IFf}}]j1fFkP{tYf[hQWW@@@@@@@@@@@@@@@@@@@@@@@WWV, `          
               :YkbGwm#@#:  `/?!?Vm#@W@W@W@@WM8N{`````:CN0#@MMBB#88B@@@M#NDUaawI}]fVVaPpN88M@@@@@@@@@@@@@@@@@@@@@@@@@@3 `           
             ` ;&lt;+CC1JP@V   ,_,`,&lt;1V$dUU$ADg6w#?       ;&amp;AZ3kk3ZwwwwUKEbE3yICIaqAAqE08PVFffwB@@@@@@@@@@@@@@@@@@@@@W@NY: `           
                \$Z3JfI8/ `      :!vk$ZUKbQZCA}-`      !K@Wh+[fFIFC1+!!/;~__!/rLYCVkV$//!!(tN@@@@@@@@@@@@@@@@@@@@@W@U  `            
              `  /FA1?bd. ```` ` :/fPOZwV1"'/, `     ` :YZNI;^//!(=]tL++&lt;(/;";;!&lt;c]!V?.~;/&lt;YQ@W@@@@@@@@@@@@@@@@@@@W@w `             
                  'v?!!!         '^\*(*/' ';` `         ,!?wf/:,'':,__:'..,::::^/!!c]``,_/*+FN@W@@@@@@@@@@@@@@@@@@@@F `             
                ` `".;!.,,,.. `` .,.,_!!~_,  ` ```````` `:c1Y1&lt;(/:'..`.`` .,,::~!/(/``.::/!&lt;lUW@@@@@@@@@@@@WW@@@@@W@( `             
                  `/;:, ` `,':':':,~/!!!!&lt;-  `         ` .!v]l&lt;!!;;;_,,':,::_:_::'`  ,_;/!r&lt;=vPN@@@@@@@@@@@@@@@@@W@d  `             
                   /&lt;^,         `.,;!?fV$!    ```   `:^!_, `,&lt;VI&lt;!/:',.` ```     `.:,~/!(&lt;==L?lC0@@@@@@@@@W#8WMKGMN' `              
                   -!\' ` ```  ,:/(+lIkPI``!1!'&lt;IIffFZmmDw?!!vUwVIIv&lt;r/";:_,.',.`.',:"!\(+?cvj1YVDNMB@WWW@BKaNy&lt;%@I                 
                    .", `    '!/+c1Co%It[|&lt;FV}/'*%Kb8@@@@@BN6Z=;/+fI}J?(\!//!!";""^//!(!!&lt;&lt;[f}fFVkd8WBW@@#bK8I^?N@! `               
                     `      `!fCF%wPI?*\!~'` `/==F6N@#8gQEAV[!,,,_/&lt;v}{Jt]=((!!!*\(&lt;++?(*(+YfII}Fw$mQwIkkf}V?:!Q@&amp;  `               
                          `  t$w%kZf\/,,``   "IQM@@WN0$w}l+//"!;:,_:;!?v1FF}[=c[lL[]vc&lt;+&lt;&lt;lLY}fFVkUF?\/!/*!,'?0@@@N* `              
                          ` 'fZj]Y!_,       `,/vJ][]=!/!__:,,':"//!*/!?fflLCFCfJt11vv|??cvl]FIFo%V3F!;:::'_?A@@@mPQF' `             
                          ```:^_&lt;&lt;!lF!`` ,,':+FIFIFF}}F{jc?&lt;(!!!(=FZPyFfIy++[[Yfffv&lt;&lt;&lt;&lt;L=YJfJFIFwZN8aI1YVD#@@@@F  ` `               
                          `  ` .\jVpB@8mgw(!\JV3wUmmpQm0N8N8M@@@N8W@@M6y]+!//r]J}fLLl?Lt11v[fFykZdW@@@@@@@@@#]:  ` `                
                          `     *L1FyVI&lt;~ `    ` ,:::::,:_!!!(+[f1?1IFfl(!"//!&lt;jf=L]cLfY[]1Fyyk$Gg@WWWWW@@@0,  `` `                 
                             `,~Y}Yt&lt;:`      ~+FVZ%IIFJ=!!"/;_"::_:_;&lt;cLLl&lt;+&lt;?}VF1YJ1f1fFVyVwZKQN#B@WW@W@@@! ``                     
                           ` `/(vZI!`  `` ` ,?}IIIIFfFyVFJfL?!//;"^/!*=}}1tCF%VwhyFIFFIyVkaAEQ8N8m0N@@@@@@A`                        
                            -_!/!=/. ``     ```':::;"";~":___;^/!""/!!*=JFFykZVIyVFyhPZkZpg08M@#daGm8AmQG[  `                       
                            ` /!;'``              `  ```  ` `,::"!"/(?v{CIVkw$GVVw3Z$dpmgN#MW@@6awZUD\     `                        
                              .";   ``      ` `` --..   `   `` :;!!!(?vfFIkZUKK$AOD00g8NBWW@WBN$w%wPB@g!   ```                      
                            ` '',;'`          ':~;"_~:'_::'_:"(c?r+1FoP3$Emmm06D0gNBW@W@@@W@@8APwVVQ@W@@p(     ``````               
                            ``I* ,~;: `    .,*?[=?L=L=??+?[1fIFVV%Zb68M#NNBBBB@M@@W@@@@@@W@N8QakyyUW@@WW@@#w!.`       ``````        
                          ```|@V   `,,.`,:/?JVUZPU3PZKdKaDN##88N88#B@@@@@W@@@W@@@@WW@@@@B#8NmUwV%w8@@@@@@W@@@6F?\"_,         ```````
                  ```    `  &lt;M@0`     ` -,!JVQB@@@B@W@@@W@@@@@@@@@@@@@@WW@B@@@@WWWBB@@BBNNgpK3PZP&amp;@@@@@@@@W@W@#QEQ06NAyv!:-         
             `````    ``  `/mW@@" ` `   `.,:!(y8B@@@@@@@@@WW@W@W@W@@@M#BW@BBBB@@@MBMB@#N#8gQAZwGg@@@@@@@@@@@@W@BpUakg@@@@W#&amp;dVJ&lt;/,. 
          ``      ,,.`   ,*wb@W@F `     ` :~"~/vVgB@@WWWWWWW@W@@@BB#@W@WWW@@@@WMMBB@WBN#Ngmd$Ud8W@@@@@@@@@@@@@W@W&amp;ak3wN@@N#BM@@@@B&amp;Q
      ```    ._+I$w!`  :!?JCP@@@N` `      ':!/(!&lt;CPp#@W@@WWB@BB@BBWWW@@B@B@@B@W@BWW@#8NgQEGbDgB@@@@@BW@@@@@@@@WW@@8wCyVaW@@BN8#N#B@@
   ``     ~[Z8@@8+  .':&lt;|c+L$W@@@&lt;      `  :"!(+&lt;LfIUNB@W@@W@M@@BWWMNBBBBM@@BB@BB#N88&amp;mEGUAp8@@W@@M#8#@@@@@@@@@WW@@#UJ}}f0@@BN#NMBM#
``     ;v$8@@@@I   !!!/r&lt;r/?0@W@@8_  ``  `,':;!(+=[FZQ8N#@@MBM@@@BBMMBBB##MB@N##8Ng8g0mmg6g8NW@@@@NNN&amp;@@@@@@@@@@WNM@B6y1FLk@@@N#8BNN
   ,/[ag@@@@@B&lt;  '/&lt;c//*!!_L#@@@W@8/`     :;!;/(=]}IPAQ8N8N#8#NB#8NBM##N#N8NN#88NN8g0gNN8N#MW@@@@@Nm888@@@@@@@@@@BN#8#N$f]rL0@WM@B#8
:/[UQ#W@@WW@N(: _=]L!*+/r/:1W@N#@@@#F;`    :";\ct}FP$Aqq088N8NNNNN#BN##8N88N8#888#8NNNN#BW@@@@@@@@@NmN6@@@@@@@@@@WB#N8N8&amp;Ut!/I#@B@@@
km0#W@@W@@@g!/.:!!!=1Cl+=^ t@N8B@@@@8EI&lt;/"_,/!&lt;=v1FUGOQmgNN8N8888####NNBBNNNMN#NMBNM##B@@W@@@@@@@@@#0&amp;&amp;NMW@@@W@@@@@WM8gmDQQZ[/v0BM#8
8MBW@@@@@@Z!l:!y+''?GPV{\, Z&amp;mB@W@@@@MNQAwfvLLY][tyZA6NN#N##N#888#NB#NN#N##NNNBN#N##B#MBW@@@@@@@@@@@gK&amp;A8#@@WWWM#B@@@@@N8g8N8V*Y0W#M
NB@@WW@@My"+:!oVI&lt;/c[Y]!!!fBCg@@@@@@@@@N8N0qGaPVIVVkAQ&amp;N88#88888#NN@@MB###BN#NN8#BB#BB@W@@@@@@@@@@@@8KE$db8W@W@B#8NW@@@@@88&amp;88k=Jbp@
BW@@@@WNF/?;?KUqK}LFff}!+F8AZ@@@@@@@@@WW@##NggggmE$AQ6g8N8NNN8NN88#BBMM#N888N8#N#BW@W@@@@@@W@W@@@@@@#QK8PNNW@@W@@@WW@@@W@W#88g&amp;F+%yN
@@@@@NKf/(!fPFvc=tUg$Fc:~[8y0@@@@@@@@@MMBWW@M#N8&amp;g6g88N8N888NNNNNN#MBNN#N8NN8N8#N#@W@@W@@@@@@@@W@@@@#PIaJw@W@@@@@@@@@@@@@@@@BN8g}LVD
@@@@Wat!!_Ch\/&lt;IU&amp;@@BU?',&lt;$V#@@@@@@@@@NNN@@@W@#8Nggg&amp;&amp;gNN8#NNNNN#B#MBNNNN8N8g8NN#WB@@W@@@@@NKQ@@@@@@Bk]Fg8@W@@@@@@@@@@@@@@@@@@@@#wyd
@@W@$I/,,lav(lU#@@@W#E!`:wPA@@@@@@@@@@#0088#W@W@N8N88gN88N8###N#BMN#8N&amp;0mQppQQ0gNN8N#N#W@@0C1JFKB@BWBgpN@@W@@@@@@@@@@@@@@@@@@W@W@@gN
</pre>
</td>
<td>
<img src="bart-recycle.png"/>
</td>
</tr>
</table>

These programs convert between ASCII art and PNM the netpbm
way, with decent quality in each direction.  (My
asciiarttopgm, BTW, is *way* better than the asciitopgm
currently in Netpbm of any stripe.) This suite consists of
programs to convert ASCII art to PBM and PGM format
(<tt>asciiarttopbm</tt>, <tt>asciiarttopgm</tt>), and a
program to convert PGM to ASCII art
(<tt>pgmtoasciiart</tt>), along with a helper script for the
latter (<tt>mkasciiart</tt>).

For usage information, please see the included manual pages.

## History

These are really really old programs.  It turns out, after
some investigation, that I seem to have wrote them :-). I
kind of think I might have written <tt>pgmtoasciiart.c</tt>
at UO in the early 1990s. The suite has been continuously
maintained and improved since then.

## Building

A development install of Netpbm
(http://netpbm.sourceforge.net) is necessary to build this
stuff. It will build and run with the Debian
`libnetpbm11-dev` currently in `experimental`: this is
currently preferred.  It will build and run with the
Sourceforge netpbm: see the `Makefile`.  It will probably
build and run with the Debian `libnetpbm10-dev`: see the
`Makefile`.

The included `glyphshades` utility can produce or reproduce
shade files for font density information. Rebuilding
`glyphshades` requires a development install of
[Cairo](http://cairographics.org) (Debian package
`libcairo-dev`). This is not necessary to build the other
tools, which will otherwise use glyph shade files checked
into the repo.

This stuff represents a crazy amount of tweaking. Have fun
doing more if you like.

## License

This work is licensed under the "MIT License". Please see
the file `COPYING` in this distribution for license terms.
