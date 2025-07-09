#include "ugly.h"
#include "bad.h"

struct SplineImpl {

    SplineImpl(double* x, double* y, double* y2, size_t n) : x_(x), y_(y), y2_(y2), n_(n) {
    }
    ~SplineImpl() {
        delete[] x_;
        delete[] y_;
        delete[] y2_;
    }

    size_t n_;
    double* x_;
    double* y_;
    double* y2_;
};

Spline::Spline(const std::vector<double>& x, const std::vector<double>& y, double a, double b) {
    size_t np = x.size();
    double* xp{new double[np]};
    double* yp{new double[np]};
    double* y2p{new double[np]};
    for (size_t i = 0; i < np; ++i) {
        xp[i] = x[i];
        yp[i] = y[i];
    }
    mySplineSnd(xp, yp, np, a, b, y2p);
    impl_ = std::make_shared<SplineImpl>(xp, yp, y2p, np);
}

double Spline::Interpolate(double x) {
    double ret;
    mySplintCube(impl_->x_, impl_->y_, impl_->y2_, impl_->n_, x, &ret);
    return ret;
}
