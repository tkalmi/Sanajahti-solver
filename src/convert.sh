convert scrot.png             \
    -scale 1000%                              \
    -blur 1x65535 -blur 1x65535 -blur 1x65535 \
    -contrast                                 \
    -normalize                                \
    -despeckle -despeckle                     \
    -type grayscale                           \
    -sharpen 1                                \
    -posterize 3                              \
    -negate                                   \
    -gamma 100                                \
    -compress zip                             \
     scrot.png

