# Robust Distinct Sampling
Sampling from a noisy data stream. This library includes multiple algorithms, including
- algorithm that returns distinct samples from a noisy data stream, see [robust_sampler_test](https://github.com/jiecchen/StreamingCC/blob/master/src/robust_l0_sampling/tests/robust_sampler_test.cc) for example.
- algorithm that estimates the number of groups (treat close points as one group) in the data stream, see [f0_test](https://github.com/jiecchen/StreamingCC/blob/master/src/robust_l0_sampling/tests/f0_test.cc) for example.

## TODO
- clean up the code
- remove unnecessary dependencies
- unify the interfaces
