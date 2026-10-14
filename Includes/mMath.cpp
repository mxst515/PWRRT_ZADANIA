#include "mMath.h"
namespace mMath {
    double calc_det(sPoint p, sPoint q, sPoint r) {
        // |xp yp 1|
        // |xq yq 1|
        // |xr yr 1|
        // compute the det of matrix of vector points p q r

        return (p.x * q.y + p.y * r.x + q.x * r.y) - (q.y * r.x + p.x * r.y + p.y * q.x);
    }
}
