
#lib-SDKPlugin

一个第三方sdk插件的集合(Android / iOS平台)，是用cocos2d-x开发游戏时，接入第三方sdk的一个整合包，为了方便第三方sdk的二次接入，和删除。

#SDK列表
目前已包含的sdk如下
- AppStore
- GooglePlay
- DataEye
- TalkingData
- Facebook (login/share/invite)
- 移动MM Pay

#说明
目前该项目已弃用，不在更新，将会有新的lib代替。
该项目是以源代码的形式直接加入到你的工程里，所以在添加/删除时，会在项目工程里对不同平台的资源进行添加/删除操作，当多个sdk资源混合在一起时，很容易混淆不清，导致移除某一sdk时很麻烦，加大了sdk间的耦合度。
新的解决方案是，不同的sdk以各自平台的lib库形式加入到项目工程中，那么删除/添加资源或源文件时，就只用关心自己这个lib即可。然后提供一个对这些lib统一管理的管理类即可。新的repository将在不久后上传。
