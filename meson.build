project('ALSA Examples', 'c')

cc = meson.get_compiler('c')

alsa = cc.find_library('asound', required : true)

common_incl = include_directories('common')

subdir('common')
subdir('capture')
subdir('playback')
subdir('async-playback')
subdir('simultaneous-playback-capture')
subdir('network-streamed-playback-capture')
subdir('utils')
