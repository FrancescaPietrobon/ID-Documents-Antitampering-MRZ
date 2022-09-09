#include "../include/Characters.h"



Box::Box(unsigned x_, unsigned y_, unsigned h_, unsigned w_, unsigned label_):
    x(x_), y(y_), h(h_), w(w_), label(label_) {letter = dictionary[label_];}

