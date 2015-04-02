# Perception-IME

Perception-IME is a dedicated support stack for Typed Language Support,

This means that Languages such as Japanese which are full of same-sounding words with many readings or difficult to enter due to requirement of extended input rules per-language become accessible to the everyday user.

How this is accomplished however is another story.

# Keyboard Input Extensions -

Primarily this is acheived through the standard AmigaOS "locale.library" subsystem.
This subsystem includes a "Sys:Prefs/Locale" Settings Editor.

Any Language selected within this editor as the first language preferred by the owner of the system may request use of perception.library.
When a "#?.language" module is loaded during the OS startup and makes use of perception.library then perception.library will launch an Input Method Editor wrapping process.

For the technically minded,  a dependency reference would be like this...
Locale.Library -> my_favorite.language -> perception.library(+Perception-IME process).

However when "my_favorite.language" uses the Registration mechanism of perception.library it is no longer possible to "unload" the language or Perception-IME.

You can change your favorite language but the IME settings remain preferential to the current boot language layout until the machine is rebooted.
