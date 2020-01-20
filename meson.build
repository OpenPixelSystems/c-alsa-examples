project('ALSA Examples', 'c')

cc = meson.get_compiler('c')

alsa = cc.find_library('asound', required : true)

subdir('capture')
subdir('playback')
subdir('async-playback')
subdir('simultaneous-playback-capture')
subdir('network-streamed-playback-capture')
