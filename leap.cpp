
// calendar_spectral.cpp
// 编译: g++ -std=c++17 -O2 calendar_spectral.cpp -o cal_spec

#include <iostream>
#include <iomanip>
#include <vector>
#include <deque>
#include <complex>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <set>
#include <random>
#include <functional>
#include <limits>
#include <stdexcept>
#include <initializer_list>
#include <utility>

using Complex = std::complex<double>;
using ComplexVector = std::vector<Complex>;
using RealVector = std::vector<double>;
constexpr double PI = 3.14159265358979323846;

// =====================================================================
// Window
// =====================================================================
class Window {
public:
    enum class Type {
        Rectangular,
        Hann,
        Hamming,
        Blackman,
        Kaiser,
        FlatTop
    };

    static double besselI0(double value) {
        double sum = 1.0;
        double term = 1.0;
        double halfSquare = value * value / 4.0;
        for (int order = 1; order < 64; ++order) {
            term *= halfSquare / static_cast<double>(order * order);
            sum += term;
            if (term < 1e-16 * sum) {
                break;
            }
        }
        return sum;
    }

    static RealVector make(Type type, size_t length, double parameter = 8.0) {
        RealVector window(length, 1.0);
        if (length <= 1) {
            return window;
        }
        double denominator = static_cast<double>(length - 1);
        for (size_t index = 0; index < length; ++index) {
            double angle = 2.0 * PI * static_cast<double>(index) / denominator;
            switch (type) {
                case Type::Rectangular: {
                    window[index] = 1.0;
                    break;
                }
                case Type::Hann: {
                    window[index] = 0.5 - 0.5 * std::cos(angle);
                    break;
                }
                case Type::Hamming: {
                    window[index] = 0.54 - 0.46 * std::cos(angle);
                    break;
                }
                case Type::Blackman: {
                    window[index] = 0.42
                        - 0.5 * std::cos(angle)
                        + 0.08 * std::cos(2.0 * angle);
                    break;
                }
                case Type::Kaiser: {
                    double normalized = 2.0 * static_cast<double>(index) / denominator - 1.0;
                    double inner = std::sqrt(std::max(0.0, 1.0 - normalized * normalized));
                    window[index] = besselI0(parameter * inner) / besselI0(parameter);
                    break;
                }
                case Type::FlatTop: {
                    window[index] = 0.21557895
                        - 0.41663158 * std::cos(angle)
                        + 0.277263158 * std::cos(2.0 * angle)
                        - 0.083578947 * std::cos(3.0 * angle)
                        + 0.006947368 * std::cos(4.0 * angle);
                    break;
                }
            }
        }
        return window;
    }

    static double sum(const RealVector& window) {
        double total = 0.0;
        for (size_t index = 0; index < window.size(); ++index) {
            total += window[index];
        }
        return total;
    }
};

// =====================================================================
// Epicycle
// =====================================================================
class Epicycle {
public:
    double amplitude = 0.0;
    double omega = 0.0;
    double phase = 0.0;

    Epicycle() = default;
    Epicycle(double amplitudeValue, double omegaValue, double phaseValue)
        : amplitude(amplitudeValue), omega(omegaValue), phase(phaseValue) {}

    Complex evaluate(double position) const {
        return amplitude * std::polar(1.0, omega * position + phase);
    }

    Epicycle derivate() const {
        return Epicycle(amplitude * omega, omega, phase + PI / 2.0);
    }

    Complex derivate(double position) const {
        return Complex(0.0, 1.0) * amplitude * omega
            * std::polar(1.0, omega * position + phase);
    }

    Epicycle integral() const {
        if (std::abs(omega) < 1e-300) {
            return Epicycle(0.0, 0.0, 0.0);
        }
        return Epicycle(amplitude / omega, omega, phase - PI / 2.0);
    }

    Complex derivate(double start, double end) const {
        double span = end - start;
        if (std::abs(span) < 1e-300) {
            return derivate(start);
        }
        return (evaluate(end) - evaluate(start)) / span;
    }
};

// =====================================================================
// Waveform
// =====================================================================
class Waveform {
public:
    std::deque<Epicycle> epicycles;

    Waveform() = default;
    Waveform(std::initializer_list<Epicycle> values) : epicycles(values) {}

    void add(const Epicycle& epicycle) {
        epicycles.push_back(epicycle);
    }

    size_t size() const {
        return epicycles.size();
    }

    Complex evaluate(double position) const {
        Complex total(0.0, 0.0);
        for (size_t index = 0; index < epicycles.size(); ++index) {
            total += epicycles[index].evaluate(position);
        }
        return total;
    }

    Waveform derivate() const {
        Waveform result;
        for (size_t index = 0; index < epicycles.size(); ++index) {
            result.add(epicycles[index].derivate());
        }
        return result;
    }

    Complex derivate(double position) const {
        Complex total(0.0, 0.0);
        for (size_t index = 0; index < epicycles.size(); ++index) {
            total += epicycles[index].derivate(position);
        }
        return total;
    }

    Waveform integral() const {
        Waveform result;
        for (size_t index = 0; index < epicycles.size(); ++index) {
            result.add(epicycles[index].integral());
        }
        return result;
    }

    Complex derivate(double start, double end) const {
        Complex total(0.0, 0.0);
        for (size_t index = 0; index < epicycles.size(); ++index) {
            total += epicycles[index].derivate(start, end);
        }
        return total;
    }
};

// =====================================================================
// Sample
// =====================================================================
class Sample {
public:
    double duration = 0.0;
    double count = 0.0;

    Sample() = default;
    Sample(double durationValue, double countValue)
        : duration(durationValue), count(countValue) {}

    double samplingRate() const {
        if (duration > 0.0) {
            return count / duration;
        }
        return 0.0;
    }

    double samplingInterval() const {
        double rate = samplingRate();
        if (rate > 0.0) {
            return 1.0 / rate;
        }
        return 0.0;
    }

    size_t pointCount() const {
        long long rounded = static_cast<long long>(std::llround(count));
        if (rounded < 0) {
            return 0;
        }
        return static_cast<size_t>(rounded);
    }
};

// =====================================================================
// Sampler —— 依据 samples 采样后直接变换，不设对外辅助外壳
// =====================================================================
class Sampler {
public:
    std::vector<Sample> samples;
    Waveform source;
    ComplexVector directData;

    Sampler() = default;
    Sampler(std::initializer_list<Sample> sampleList)
        : samples(sampleList) {}
    Sampler(std::initializer_list<Sample> sampleList, const Waveform& sourceWaveform)
        : samples(sampleList), source(sourceWaveform) {}
    explicit Sampler(const std::vector<Sample>& sampleList)
        : samples(sampleList) {}
    Sampler(const std::vector<Sample>& sampleList, const Waveform& sourceWaveform)
        : samples(sampleList), source(sourceWaveform) {}

    void add(const Sample& sample) {
        samples.push_back(sample);
    }

    void setSource(const Waveform& waveform) {
        source = waveform;
    }

    void setDirectData(const ComplexVector& data) {
        directData = data;
    }

    void clearDirectData() {
        directData.clear();
    }

    size_t segmentCount() const {
        return samples.size();
    }

    size_t totalPointCount() const {
        size_t total = 0;
        for (size_t index = 0; index < samples.size(); ++index) {
            total += samples[index].pointCount();
        }
        return total;
    }

    double totalDuration() const {
        double total = 0.0;
        for (size_t index = 0; index < samples.size(); ++index) {
            total += samples[index].duration;
        }
        return total;
    }

    RealVector sampleTimes() const {
        RealVector times;
        double currentTime = 0.0;
        for (size_t segmentIndex = 0; segmentIndex < samples.size(); ++segmentIndex) {
            const Sample& sample = samples[segmentIndex];
            size_t pointCount = sample.pointCount();
            double interval = sample.samplingInterval();
            for (size_t pointIndex = 0; pointIndex < pointCount; ++pointIndex) {
                times.push_back(currentTime + static_cast<double>(pointIndex) * interval);
            }
            currentTime += sample.duration;
        }
        return times;
    }

    ComplexVector gatherComplex() const {
        if (!directData.empty()) {
            return directData;
        }
        RealVector times = sampleTimes();
        ComplexVector output(times.size(), Complex(0.0, 0.0));
        for (size_t index = 0; index < times.size(); ++index) {
            output[index] = source.evaluate(times[index]);
        }
        return output;
    }

    RealVector gatherReal() const {
        if (!directData.empty()) {
            RealVector output(directData.size(), 0.0);
            for (size_t index = 0; index < directData.size(); ++index) {
                output[index] = directData[index].real();
            }
            return output;
        }
        RealVector times = sampleTimes();
        RealVector output(times.size(), 0.0);
        for (size_t index = 0; index < times.size(); ++index) {
            output[index] = source.evaluate(times[index]).real();
        }
        return output;
    }

    // ---------------- 对外变换：无参，内部依据 samples 采样 ----------------
    ComplexVector radix2(bool inverse = false) const {
        return radix2Impl(gatherComplex(), inverse);
    }

    ComplexVector radix3(bool inverse = false) const {
        ComplexVector data = gatherComplex();
        if (!isPowerOf(data.size(), 3)) {
            throw std::invalid_argument("radix3 requires length to be a power of 3");
        }
        mixedRadixImpl(data, inverse, 3);
        return data;
    }

    ComplexVector radix5(bool inverse = false) const {
        ComplexVector data = gatherComplex();
        if (!isPowerOf(data.size(), 5)) {
            throw std::invalid_argument("radix5 requires length to be a power of 5");
        }
        mixedRadixImpl(data, inverse, 5);
        return data;
    }

    ComplexVector radix7(bool inverse = false) const {
        ComplexVector data = gatherComplex();
        if (!isPowerOf(data.size(), 7)) {
            throw std::invalid_argument("radix7 requires length to be a power of 7");
        }
        mixedRadixImpl(data, inverse, 7);
        return data;
    }

    ComplexVector mixedRadix(bool inverse = false) const {
        ComplexVector data = gatherComplex();
        mixedRadixImpl(data, inverse, 0);
        return data;
    }

    ComplexVector blueStein(bool inverse = false) const {
        return blueSteinImpl(gatherComplex(), inverse);
    }

    ComplexVector discreteFourierTransform(bool inverse = false) const {
        ComplexVector data = gatherComplex();
        directTransform(data, inverse);
        return data;
    }

    ComplexVector fastFourierTransform(bool inverse = false) const {
        ComplexVector input = gatherComplex();
        size_t length = input.size();
        if (length <= 1) {
            return input;
        }
        if (isPowerOf(length, 2)) {
            return radix2Impl(input, inverse);
        }
        if (isSmooth(length)) {
            ComplexVector data = input;
            mixedRadixImpl(data, inverse, 0);
            return data;
        }
        return blueSteinImpl(input, inverse);
    }

    ComplexVector waveletTransform() const {
        RealVector input = gatherReal();
        size_t length = input.size();
        ComplexVector output(length, Complex(0.0, 0.0));
        if (length == 0) {
            return output;
        }
        size_t half = length / 2;
        for (size_t index = 0; index < half; ++index) {
            double firstValue = input[2 * index];
            double secondValue = input[2 * index + 1];
            output[index] = Complex((firstValue + secondValue) / std::sqrt(2.0), 0.0);
            output[half + index] = Complex((firstValue - secondValue) / std::sqrt(2.0), 0.0);
        }
        if (length % 2 != 0) {
            output[length - 1] = Complex(input[length - 1], 0.0);
        }
        return output;
    }

    ComplexVector laplacianTransform() const {
        RealVector input = gatherReal();
        size_t length = input.size();
        ComplexVector output(length, Complex(0.0, 0.0));
        if (length == 0) {
            return output;
        }
        if (length == 1) {
            output[0] = Complex(0.0, 0.0);
            return output;
        }
        output[0] = Complex(input[1] - input[0], 0.0);
        for (size_t index = 1; index + 1 < length; ++index) {
            double value = input[index - 1] - 2.0 * input[index] + input[index + 1];
            output[index] = Complex(value, 0.0);
        }
        output[length - 1] = Complex(input[length - 2] - input[length - 1], 0.0);
        return output;
    }

private:
    bool isPowerOf(size_t value, size_t base) const {
        if (value == 0) {
            return false;
        }
        while (value % base == 0) {
            value /= base;
        }
        return value == 1;
    }

    bool isSmooth(size_t value) const {
        if (value == 0) {
            return false;
        }
        while (value % 2 == 0) {
            value /= 2;
        }
        while (value % 3 == 0) {
            value /= 3;
        }
        while (value % 5 == 0) {
            value /= 5;
        }
        while (value % 7 == 0) {
            value /= 7;
        }
        return value == 1;
    }

    size_t smallestRadixFactor(size_t value) const {
        if (value % 2 == 0) {
            return 2;
        }
        if (value % 3 == 0) {
            return 3;
        }
        if (value % 5 == 0) {
            return 5;
        }
        if (value % 7 == 0) {
            return 7;
        }
        return 1;
    }

    ComplexVector radix2Impl(const ComplexVector& input, bool inverse) const {
        size_t length = input.size();
        if (length <= 1) {
            return input;
        }
        if (!isPowerOf(length, 2)) {
            throw std::invalid_argument("radix2 requires length to be a power of 2");
        }
        ComplexVector data = input;
        for (size_t firstIndex = 1, secondIndex = 0; firstIndex < length; ++firstIndex) {
            size_t bit = length >> 1;
            for (; secondIndex & bit; bit >>= 1) {
                secondIndex ^= bit;
            }
            secondIndex ^= bit;
            if (firstIndex < secondIndex) {
                std::swap(data[firstIndex], data[secondIndex]);
            }
        }
        for (size_t stage = 2; stage <= length; stage <<= 1) {
            double angle = 2.0 * PI / static_cast<double>(stage) * (inverse ? 1.0 : -1.0);
            Complex twiddleStep(std::cos(angle), std::sin(angle));
            for (size_t block = 0; block < length; block += stage) {
                Complex twiddle(1.0, 0.0);
                for (size_t offset = 0; offset < stage / 2; ++offset) {
                    Complex even = data[block + offset];
                    Complex odd = data[block + offset + stage / 2] * twiddle;
                    data[block + offset] = even + odd;
                    data[block + offset + stage / 2] = even - odd;
                    twiddle *= twiddleStep;
                }
            }
        }
        if (inverse) {
            double scale = 1.0 / static_cast<double>(length);
            for (size_t index = 0; index < length; ++index) {
                data[index] *= scale;
            }
        }
        return data;
    }

    ComplexVector blueSteinImpl(const ComplexVector& input, bool inverse) const {
        size_t length = input.size();
        if (length <= 1) {
            return input;
        }
        if (isSmooth(length)) {
            ComplexVector data = input;
            mixedRadixImpl(data, inverse, 0);
            return data;
        }
        size_t expandedLength = 1;
        while (expandedLength < 2 * length - 1) {
            expandedLength <<= 1;
        }
        ComplexVector first(expandedLength, Complex(0.0, 0.0));
        ComplexVector second(expandedLength, Complex(0.0, 0.0));
        double sign = inverse ? 1.0 : -1.0;
        long long doubledModulus = 2LL * static_cast<long long>(length);
        for (size_t index = 0; index < length; ++index) {
            long long squareIndex = static_cast<long long>(index) * static_cast<long long>(index)
                % doubledModulus;
            double angle = sign * PI * static_cast<double>(squareIndex) / static_cast<double>(length);
            Complex rotation(std::cos(angle), std::sin(angle));
            first[index] = input[index] * rotation;
            second[index] = std::conj(rotation);
            if (index != 0) {
                second[expandedLength - index] = std::conj(rotation);
            }
        }
        first = radix2Impl(first, false);
        second = radix2Impl(second, false);
        for (size_t index = 0; index < expandedLength; ++index) {
            first[index] *= second[index];
        }
        first = radix2Impl(first, true);
        ComplexVector output(length);
        double normalization = inverse ? 1.0 / static_cast<double>(length) : 1.0;
        for (size_t index = 0; index < length; ++index) {
            long long squareIndex = static_cast<long long>(index) * static_cast<long long>(index)
                % doubledModulus;
            double angle = sign * PI * static_cast<double>(squareIndex) / static_cast<double>(length);
            Complex rotation(std::cos(angle), std::sin(angle));
            output[index] = first[index] * rotation * normalization;
        }
        return output;
    }

    void directTransform(ComplexVector& data, bool inverse) const {
        size_t length = data.size();
        if (length <= 1) {
            return;
        }
        ComplexVector output(length);
        double sign = inverse ? 1.0 : -1.0;
        for (size_t frequencyIndex = 0; frequencyIndex < length; ++frequencyIndex) {
            Complex sum(0.0, 0.0);
            for (size_t position = 0; position < length; ++position) {
                double angle = sign * 2.0 * PI
                    * static_cast<double>(frequencyIndex)
                    * static_cast<double>(position)
                    / static_cast<double>(length);
                Complex rotation(std::cos(angle), std::sin(angle));
                sum += data[position] * rotation;
            }
            output[frequencyIndex] = sum;
        }
        if (inverse) {
            double scale = 1.0 / static_cast<double>(length);
            for (size_t index = 0; index < length; ++index) {
                output[index] *= scale;
            }
        }
        data = output;
    }

    void mixedRadixImpl(ComplexVector& data, bool inverse, size_t forcedRadix) const {
        size_t length = data.size();
        if (length <= 1) {
            return;
        }
        if (length <= 7) {
            directTransform(data, inverse);
            return;
        }
        size_t radix = forcedRadix;
        if (radix == 0) {
            radix = smallestRadixFactor(length);
        }
        if (radix <= 1 || length % radix != 0) {
            directTransform(data, inverse);
            return;
        }
        size_t subLength = length / radix;
        std::vector<ComplexVector> subSequences(radix, ComplexVector(subLength));
        for (size_t remainderIndex = 0; remainderIndex < radix; ++remainderIndex) {
            for (size_t position = 0; position < subLength; ++position) {
                subSequences[remainderIndex][position] = data[remainderIndex + position * radix];
            }
            mixedRadixImpl(subSequences[remainderIndex], inverse, forcedRadix);
        }
        double sign = inverse ? 1.0 : -1.0;
        for (size_t remainderIndex = 0; remainderIndex < radix; ++remainderIndex) {
            for (size_t frequencyIndex = 0; frequencyIndex < subLength; ++frequencyIndex) {
                double angle = sign * 2.0 * PI
                    * static_cast<double>(remainderIndex)
                    * static_cast<double>(frequencyIndex)
                    / static_cast<double>(length);
                Complex twiddle(std::cos(angle), std::sin(angle));
                subSequences[remainderIndex][frequencyIndex] *= twiddle;
            }
        }
        for (size_t frequencyIndex = 0; frequencyIndex < subLength; ++frequencyIndex) {
            ComplexVector group(radix);
            for (size_t remainderIndex = 0; remainderIndex < radix; ++remainderIndex) {
                group[remainderIndex] = subSequences[remainderIndex][frequencyIndex];
            }
            directTransform(group, inverse);
            for (size_t outputIndex = 0; outputIndex < radix; ++outputIndex) {
                data[outputIndex * subLength + frequencyIndex] = group[outputIndex];
            }
        }
    }
};

// =====================================================================
// Spectrum
// =====================================================================
class Spectrum {
public:
    RealVector frequencies;
    ComplexVector coefficients;

    size_t size() const {
        return frequencies.size();
    }

    bool empty() const {
        return frequencies.empty();
    }

    void clear() {
        frequencies.clear();
        coefficients.clear();
    }

    void add(double frequency, const Complex& coefficient) {
        frequencies.push_back(frequency);
        coefficients.push_back(coefficient);
    }

    RealVector magnitudes() const {
        RealVector output(size());
        for (size_t index = 0; index < size(); ++index) {
            output[index] = std::abs(coefficients[index]);
        }
        return output;
    }
};

// =====================================================================
// Leak
// =====================================================================
class Leak {
public:
    static double interpolatePeakOffset(const ComplexVector& coefficients, size_t peakIndex) {
        if (peakIndex == 0 || peakIndex + 1 >= coefficients.size()) {
            return 0.0;
        }
        double leftValue = std::log(std::abs(coefficients[peakIndex - 1]) + 1e-300);
        double centerValue = std::log(std::abs(coefficients[peakIndex]) + 1e-300);
        double rightValue = std::log(std::abs(coefficients[peakIndex + 1]) + 1e-300);
        double denominator = leftValue - 2.0 * centerValue + rightValue;
        if (std::abs(denominator) < 1e-12) {
            return 0.0;
        }
        double offset = 0.5 * (leftValue - rightValue) / denominator;
        if (offset < -0.5) {
            offset = -0.5;
        }
        if (offset > 0.5) {
            offset = 0.5;
        }
        return offset;
    }

    static double sidelobeRatio(const RealVector& magnitudes, size_t peakIndex, size_t halfWidth) {
        if (magnitudes.empty() || peakIndex >= magnitudes.size()) {
            return 0.0;
        }
        double peakValue = magnitudes[peakIndex];
        if (peakValue <= 0.0) {
            return 0.0;
        }
        double sideValue = 0.0;
        for (size_t index = 0; index < magnitudes.size(); ++index) {
            bool insideMainLobe = (index + halfWidth >= peakIndex) && (index <= peakIndex + halfWidth);
            if (!insideMainLobe) {
                if (magnitudes[index] > sideValue) {
                    sideValue = magnitudes[index];
                }
            }
        }
        return sideValue / peakValue;
    }

    static double leakageRatio(const RealVector& magnitudes, size_t peakIndex, size_t halfWidth) {
        double totalEnergy = 0.0;
        double mainLobeEnergy = 0.0;
        for (size_t index = 0; index < magnitudes.size(); ++index) {
            double energy = magnitudes[index] * magnitudes[index];
            totalEnergy += energy;
            bool insideMainLobe = (index + halfWidth >= peakIndex) && (index <= peakIndex + halfWidth);
            if (insideMainLobe) {
                mainLobeEnergy += energy;
            }
        }
        return (totalEnergy - mainLobeEnergy) / (totalEnergy + 1e-300);
    }
};

// =====================================================================
// Leap
// =====================================================================
class Leap {
public:
    static std::pair<long long, long long> continuedFraction(double ratio, long long maximumDenominator) {
        long long previousNumerator = 0;
        long long previousDenominator = 1;
        long long currentNumerator = 1;
        long long currentDenominator = 0;
        double value = ratio;
        for (int iteration = 0; iteration < 64; ++iteration) {
            long long integerPart = static_cast<long long>(std::floor(value));
            long long nextNumerator = integerPart * currentNumerator + previousNumerator;
            long long nextDenominator = integerPart * currentDenominator + previousDenominator;
            if (nextDenominator > maximumDenominator || nextDenominator <= 0) {
                break;
            }
            previousNumerator = currentNumerator;
            previousDenominator = currentDenominator;
            currentNumerator = nextNumerator;
            currentDenominator = nextDenominator;
            double fractionalPart = value - static_cast<double>(integerPart);
            if (fractionalPart < 1e-12) {
                break;
            }
            value = 1.0 / fractionalPart;
        }
        if (currentDenominator == 0) {
            return {0, 1};
        }
        return {currentNumerator, currentDenominator};
    }

    static size_t selectAnalysisWindow(const RealVector& frequencies,
                                       const RealVector& weights,
                                       double samplingRate,
                                       size_t minimumWindow,
                                       size_t maximumWindow) {
        if (frequencies.empty() || maximumWindow <= minimumWindow) {
            return maximumWindow;
        }
        std::set<size_t> candidates;
        for (size_t componentIndex = 0; componentIndex < frequencies.size(); ++componentIndex) {
            double normalizedFrequency = frequencies[componentIndex] / samplingRate;
            if (normalizedFrequency <= 0.0 || normalizedFrequency >= 0.5) {
                continue;
            }
            std::pair<long long, long long> approximation =
                continuedFraction(normalizedFrequency, static_cast<long long>(maximumWindow));
            long long denominator = approximation.second;
            if (denominator <= 0) {
                continue;
            }
            for (size_t multiplier = 1;
                 multiplier * static_cast<size_t>(denominator) <= maximumWindow;
                 ++multiplier) {
                size_t candidate = multiplier * static_cast<size_t>(denominator);
                if (candidate >= minimumWindow) {
                    candidates.insert(candidate);
                }
            }
        }
        for (size_t offset = 0; offset < 32 && maximumWindow > minimumWindow + offset; ++offset) {
            candidates.insert(maximumWindow - offset);
        }

        size_t bestWindow = maximumWindow;
        double bestCost = std::numeric_limits<double>::max();
        for (size_t candidate : candidates) {
            if (candidate < minimumWindow || candidate > maximumWindow) {
                continue;
            }
            double cost = 0.0;
            double weightSum = 0.0;
            for (size_t componentIndex = 0; componentIndex < frequencies.size(); ++componentIndex) {
                double cycles = frequencies[componentIndex]
                    * static_cast<double>(candidate) / samplingRate;
                double fractional = cycles - std::floor(cycles);
                double distance = std::min(fractional, 1.0 - fractional);
                cost += weights[componentIndex] * distance;
                weightSum += weights[componentIndex];
            }
            if (weightSum > 0.0) {
                cost /= weightSum;
            } else {
                cost /= 1.0;
            }
            if (cost < bestCost) {
                bestCost = cost;
                bestWindow = candidate;
            }
        }
        return bestWindow;
    }
};

// =====================================================================
// Spectral
// =====================================================================
class Spectral {
public:
    struct Options {
        Window::Type windowType = Window::Type::Kaiser;
        double kaiserBeta = 8.0;
        double residualThreshold = 1e-3;
        bool enableVerification = true;
    };

    struct Result {
        Waveform waveform;
        size_t analysisWindow = 0;
        double residualRatio = 1.0;
        bool verified = false;
    };

    static Result estimate(const RealVector& signal,
                           double samplingRate,
                           size_t numberOfComponents,
                           const Options& options = Options()) {
        Result result;
        size_t length = signal.size();
        if (length < 8 || numberOfComponents == 0) {
            return result;
        }

        Sampler sampler;

        // Step 0: 粗估
        RealVector hannWindow = Window::make(Window::Type::Hann, length);
        ComplexVector spectralData(length);
        for (size_t index = 0; index < length; ++index) {
            spectralData[index] = Complex(signal[index] * hannWindow[index], 0.0);
        }
        sampler.setDirectData(spectralData);
        ComplexVector transformed = sampler.fastFourierTransform();
        RealVector magnitudes(length / 2 + 1);
        for (size_t index = 0; index <= length / 2; ++index) {
            magnitudes[index] = std::abs(transformed[index]);
        }
        std::vector<size_t> peakBins = findPeakBins(
            magnitudes, numberOfComponents, std::max<size_t>(2, length / 100));
        if (peakBins.empty()) {
            return result;
        }

        RealVector coarseFrequencies;
        RealVector coarseWeights;
        for (size_t binIndex : peakBins) {
            double offset = Leak::interpolatePeakOffset(transformed, binIndex);
            double frequency = (static_cast<double>(binIndex) + offset)
                * samplingRate / static_cast<double>(length);
            coarseFrequencies.push_back(frequency);
            coarseWeights.push_back(magnitudes[binIndex]);
        }

        // Step 1: 调日选窗
        size_t minimumWindow = std::max<size_t>(64, length / 2);
        size_t analysisWindow = Leap::selectAnalysisWindow(
            coarseFrequencies, coarseWeights, samplingRate, minimumWindow, length);
        result.analysisWindow = analysisWindow;

        // Step 2: 闰余校正
        RealVector window = Window::make(options.windowType, analysisWindow, options.kaiserBeta);
        ComplexVector windowedData(analysisWindow);
        for (size_t index = 0; index < analysisWindow; ++index) {
            windowedData[index] = Complex(signal[index] * window[index], 0.0);
        }
        sampler.setDirectData(windowedData);
        ComplexVector windowedSpectrum = sampler.fastFourierTransform();
        RealVector windowedMagnitudes(analysisWindow / 2 + 1);
        for (size_t index = 0; index <= analysisWindow / 2; ++index) {
            windowedMagnitudes[index] = std::abs(windowedSpectrum[index]);
        }
        std::vector<size_t> windowedPeakBins = findPeakBins(
            windowedMagnitudes, numberOfComponents, std::max<size_t>(2, analysisWindow / 100));
        if (windowedPeakBins.empty()) {
            return result;
        }

        RealVector initialFrequencies;
        for (size_t binIndex : windowedPeakBins) {
            double offset = Leak::interpolatePeakOffset(windowedSpectrum, binIndex);
            double frequency = (static_cast<double>(binIndex) + offset)
                * samplingRate / static_cast<double>(analysisWindow);
            initialFrequencies.push_back(frequency);
        }

        std::vector<size_t> order(windowedPeakBins.size());
        std::iota(order.begin(), order.end(), 0);
        std::sort(order.begin(), order.end(), [&](size_t firstIndex, size_t secondIndex) {
            return windowedMagnitudes[windowedPeakBins[firstIndex]]
                 > windowedMagnitudes[windowedPeakBins[secondIndex]];
        });

        // Step 3: 定朔精修
        RealVector remainingSignal = signal;
        Waveform waveform;
        double halfWidth = 0.5 * samplingRate / static_cast<double>(analysisWindow);
        double maximumAmplitude = 0.0;

        for (size_t orderIndex : order) {
            if (waveform.size() >= numberOfComponents) {
                break;
            }
            double centerFrequency = initialFrequencies[orderIndex];
            double lowerFrequency = std::max(1e-9, centerFrequency - halfWidth);
            double upperFrequency = std::min(samplingRate * 0.5 - 1e-9, centerFrequency + halfWidth);
            if (upperFrequency <= lowerFrequency) {
                continue;
            }
            auto objective = [&](double trialFrequency) {
                return correlationMagnitude(remainingSignal, samplingRate, trialFrequency);
            };
            double refinedFrequency = goldenSectionMaximum(
                objective, lowerFrequency, upperFrequency, 50);
            Epicycle parameters = extractParameters(
                remainingSignal, samplingRate, refinedFrequency, static_cast<double>(length));
            if (waveform.size() == 0) {
                maximumAmplitude = parameters.amplitude;
            }
            if (parameters.amplitude < 1e-4 * std::max(maximumAmplitude, 1e-12)) {
                continue;
            }
            waveform.add(parameters);
            for (size_t index = 0; index < length; ++index) {
                double time = static_cast<double>(index) / samplingRate;
                double value = parameters.amplitude
                    * std::cos(parameters.omega * time + parameters.phase);
                remainingSignal[index] -= value;
            }
        }

        // Step 4: 验历闭环
        double ratio = residualRatio(signal, samplingRate, waveform);
        result.residualRatio = ratio;
        result.verified = (ratio <= options.residualThreshold);

        if (!result.verified && options.enableVerification) {
            Options updated = options;
            updated.kaiserBeta = std::min(14.0, options.kaiserBeta + 3.0);
            updated.enableVerification = false;
            Result retry = estimate(signal, samplingRate, numberOfComponents + 1, updated);
            if (retry.residualRatio < result.residualRatio) {
                retry.verified = (retry.residualRatio <= options.residualThreshold);
                return retry;
            }
        }

        result.waveform = waveform;
        return result;
    }

private:
    static std::vector<size_t> findPeakBins(const RealVector& magnitudes,
                                            size_t numberOfPeaks,
                                            size_t minimumSeparation) {
        std::vector<std::pair<size_t, double>> candidates;
        if (magnitudes.size() < 3) {
            return std::vector<size_t>();
        }
        for (size_t index = 1; index + 1 < magnitudes.size(); ++index) {
            if (magnitudes[index] > magnitudes[index - 1]
                && magnitudes[index] > magnitudes[index + 1]) {
                candidates.emplace_back(index, magnitudes[index]);
            }
        }
        std::sort(candidates.begin(), candidates.end(),
                  [](const std::pair<size_t, double>& first,
                     const std::pair<size_t, double>& second) {
                      return first.second > second.second;
                  });
        std::vector<size_t> selected;
        for (const auto& candidate : candidates) {
            bool acceptable = true;
            for (size_t selectedBin : selected) {
                long long difference = static_cast<long long>(candidate.first)
                    - static_cast<long long>(selectedBin);
                if (difference < 0) {
                    difference = -difference;
                }
                if (static_cast<size_t>(difference) < minimumSeparation) {
                    acceptable = false;
                    break;
                }
            }
            if (acceptable) {
                selected.push_back(candidate.first);
                if (selected.size() == numberOfPeaks) {
                    break;
                }
            }
        }
        return selected;
    }

    static double correlationMagnitude(const RealVector& signal,
                                       double samplingRate,
                                       double frequency) {
        Complex step = std::polar(1.0, -2.0 * PI * frequency / samplingRate);
        Complex rotation(1.0, 0.0);
        Complex sum(0.0, 0.0);
        for (size_t index = 0; index < signal.size(); ++index) {
            sum += signal[index] * rotation;
            rotation *= step;
        }
        return std::abs(sum);
    }

    static Epicycle extractParameters(const RealVector& signal,
                                      double samplingRate,
                                      double frequency,
                                      double windowSum) {
        Complex step = std::polar(1.0, -2.0 * PI * frequency / samplingRate);
        Complex rotation(1.0, 0.0);
        Complex sum(0.0, 0.0);
        for (size_t index = 0; index < signal.size(); ++index) {
            sum += signal[index] * rotation;
            rotation *= step;
        }
        double amplitude = 2.0 * std::abs(sum) / std::max(1e-300, windowSum);
        double phase = std::arg(sum);
        double omega = 2.0 * PI * frequency;
        return Epicycle(amplitude, omega, phase);
    }

    static double goldenSectionMaximum(const std::function<double(double)>& objective,
                                       double lowerBound,
                                       double upperBound,
                                       int iterations) {
        const double goldenRatio = (std::sqrt(5.0) - 1.0) / 2.0;
        double firstPoint = upperBound - goldenRatio * (upperBound - lowerBound);
        double secondPoint = lowerBound + goldenRatio * (upperBound - lowerBound);
        double firstValue = objective(firstPoint);
        double secondValue = objective(secondPoint);
        for (int iteration = 0; iteration < iterations; ++iteration) {
            if (firstValue > secondValue) {
                upperBound = secondPoint;
                secondPoint = firstPoint;
                secondValue = firstValue;
                firstPoint = upperBound - goldenRatio * (upperBound - lowerBound);
                firstValue = objective(firstPoint);
            } else {
                lowerBound = firstPoint;
                firstPoint = secondPoint;
                firstValue = secondValue;
                secondPoint = lowerBound + goldenRatio * (upperBound - lowerBound);
                secondValue = objective(secondPoint);
            }
        }
        return 0.5 * (lowerBound + upperBound);
    }

    static double residualRatio(const RealVector& signal,
                                double samplingRate,
                                const Waveform& waveform) {
        double signalEnergy = 0.0;
        double residualEnergy = 0.0;
        for (size_t index = 0; index < signal.size(); ++index) {
            double time = static_cast<double>(index) / samplingRate;
            Complex reconstruction(0.0, 0.0);
            for (size_t epicycleIndex = 0;
                 epicycleIndex < waveform.epicycles.size();
                 ++epicycleIndex) {
                reconstruction += waveform.epicycles[epicycleIndex].evaluate(time);
            }
            double realPart = reconstruction.real();
            double residual = signal[index] - realPart;
            signalEnergy += signal[index] * signal[index];
            residualEnergy += residual * residual;
        }
        return residualEnergy / (signalEnergy + 1e-300);
    }
};

// =====================================================================
// 演示
// =====================================================================
int main() {
    Sample firstSegment(0.5, 500.0);
    Sample secondSegment(1.5, 3000.0);

    Waveform truthWaveform;
    truthWaveform.add(Epicycle(1.00, 2.0 * PI * 123.4, 0.30));
    truthWaveform.add(Epicycle(0.55, 2.0 * PI * 251.7, 1.10));
    truthWaveform.add(Epicycle(0.30, 2.0 * PI * 302.3, -0.70));

    Sampler sampler{firstSegment, secondSegment};
    sampler.setSource(truthWaveform);

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "=== 采样计划 ===\n";
    std::cout << "  segments       = " << sampler.segmentCount() << "\n";
    std::cout << "  total points   = " << sampler.totalPointCount() << "\n";
    std::cout << "  total duration = " << sampler.totalDuration() << " s\n";

    RealVector rawSignal = sampler.gatherReal();

    std::mt19937 randomEngine(42);
    std::normal_distribution<double> noiseDistribution(0.0, 0.001);
    for (size_t index = 0; index < rawSignal.size(); ++index) {
        rawSignal[index] += noiseDistribution(randomEngine);
    }

    std::cout << "\n=== 前 8 个采样点 ===\n";
    RealVector times = sampler.sampleTimes();
    for (size_t index = 0; index < 8 && index < times.size(); ++index) {
        std::cout << "  t=" << std::setw(9) << times[index]
                  << "  x=" << std::setw(9) << rawSignal[index] << "\n";
    }

    double samplingRate = firstSegment.samplingRate();
    size_t firstLength = firstSegment.pointCount();

    Sampler analysisSampler;
    analysisSampler.add(firstSegment);
    ComplexVector firstComplex(firstLength);
    for (size_t index = 0; index < firstLength; ++index) {
        firstComplex[index] = Complex(rawSignal[index], 0.0);
    }
    analysisSampler.setDirectData(firstComplex);

    ComplexVector spectrum = analysisSampler.fastFourierTransform();
    RealVector magnitudes(firstLength / 2 + 1);
    for (size_t index = 0; index <= firstLength / 2; ++index) {
        magnitudes[index] = std::abs(spectrum[index]);
    }

    std::cout << "\n=== 普通 FFT 峰值（无校正）===\n";
    std::vector<std::pair<size_t, double>> peaks;
    for (size_t index = 1; index + 1 < magnitudes.size(); ++index) {
        if (magnitudes[index] > magnitudes[index - 1]
            && magnitudes[index] > magnitudes[index + 1]) {
            peaks.emplace_back(index, magnitudes[index]);
        }
    }
    std::sort(peaks.begin(), peaks.end(),
              [](const std::pair<size_t, double>& first,
                 const std::pair<size_t, double>& second) {
                  return first.second > second.second;
              });
    size_t shown = 0;
    for (const auto& peak : peaks) {
        double frequency = static_cast<double>(peak.first)
            * samplingRate / static_cast<double>(firstLength);
        std::cout << "  frequency=" << std::setw(9) << frequency
                  << "  (bin " << peak.first << ")\n";
        ++shown;
        if (shown == truthWaveform.size()) {
            break;
        }
    }

    Spectral::Options options;
    options.windowType = Window::Type::Kaiser;
    options.kaiserBeta = 8.0;
    options.residualThreshold = 1e-4;
    options.enableVerification = true;

    Spectral::Result result = Spectral::estimate(
        rawSignal, samplingRate, truthWaveform.size(), options);

    std::cout << "\n=== 历法式频谱精估 ===\n";
    std::cout << "  analysisWindow = " << result.analysisWindow << "\n";
    std::cout << "  residualRatio  = " << result.residualRatio << "\n";
    std::cout << "  verified       = " << (result.verified ? "yes" : "no") << "\n";
    for (size_t epicycleIndex = 0;
         epicycleIndex < result.waveform.epicycles.size();
         ++epicycleIndex) {
        const Epicycle& epicycle = result.waveform.epicycles[epicycleIndex];
        double frequency = epicycle.omega / (2.0 * PI);
        std::cout << "  #" << (epicycleIndex + 1)
                  << "  frequency=" << std::setw(9) << frequency
                  << "  amplitude=" << std::setw(7) << epicycle.amplitude
                  << "  phase=" << std::setw(9) << epicycle.phase << "\n";
    }

    return 0;
}