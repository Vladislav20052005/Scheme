#include <fft.h>

#include <fftw3.h>

#define SQRT_2 1.41421356237
#define INVERSE_16 0.0625

class DctCalculator::Impl {
public:
    Impl(size_t width, std::vector<double> *input, std::vector<double> *output)
        : plan_(fftw_plan_r2r_2d(width, width, input->data(), output->data(), FFTW_REDFT01,
                                 FFTW_REDFT01, FFTW_DESTROY_INPUT)),
          input_(input->data()),
          width_(width) {
    }

    void Run() {
        for (size_t i = 0; i < width_ * width_; i += width_) {
            input_[i] *= SQRT_2;
        }
        for (size_t i = 0; i < width_; i++) {
            input_[i] *= SQRT_2;
        }
        for (size_t i = 0; i < width_ * width_; i++) {
            input_[i] *= INVERSE_16;
        }
        fftw_execute(plan_);
    }

    void Free() {
        fftw_destroy_plan(plan_);
    }

private:
    fftw_plan plan_;
    double *input_;
    size_t width_;
};

DctCalculator::DctCalculator(size_t width, std::vector<double> *input,
                             std::vector<double> *output) {
    if (input->size() != width * width || output->size() != width * width) {
        throw std::invalid_argument("Incorrect input/output size");
    }
    if (width < 1) {
        throw std::invalid_argument("width is too low");
    }
    impl_ = std::make_unique<Impl>(width, input, output);
}

void DctCalculator::Inverse() {
    impl_->Run();
}

DctCalculator::~DctCalculator() {
    impl_->Free();
}
