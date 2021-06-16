#pragma once

#include <functional>

#include "Elysium/AI/AIUtility.h"
#include "Elysium/AI/Matrix.h"

namespace Elysium
{
    class Model;

    class HiddenLayer
    {
        friend class Model;

    public:
        virtual ~HiddenLayer() = default;

    public:
        Matrix Weights;

    protected:
        virtual void forwardPass(const Matrix& inputs,
            Matrix& results) = 0;
        virtual float calculateError(const Matrix& inputs, const Matrix& outputs,
            Matrix& results, Matrix& error) = 0;
        virtual void calculateDelta(const Matrix& error, const Matrix& outputs, const Matrix& inputs,
            Matrix& delta) = 0;
        virtual void backwardPass(const Matrix& prevDelta, const Matrix& prevWeights, const Matrix& outputs, const Matrix& inputs,
            Matrix& delta) = 0;

    protected:
        float LearningRate = 0.1f;

        using MathFn = std::function<float(float)>;

        MathFn m_Activation;
        MathFn m_ActivationDerivative;
    };
}