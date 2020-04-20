project('ALSA Examples', 'c')

cc = meson.get_compiler('c')

alsa = cc.find_library('asound', required : true)
math = cc.find_library('m', required : true)

common_incl = include_directories('common')

subdir('common')
subdir('capture')
subdir('playback')
subdir('playback-nonblock')
subdir('async-playback')
subdir('async-capture')
subdir('simultaneous-playback-capture')
subdir('network-streamed-playback-capture')
subdir('utils')
