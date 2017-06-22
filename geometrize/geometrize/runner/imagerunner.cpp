#include "imagerunner.h"

#include <memory>
#include <vector>

#include "../bitmap/bitmap.h"
#include "../core.h"
#include "../model.h"
#include "../shape/shapetypes.h"
#include "../commonutil.h"
#include "imagerunneroptions.h"

namespace geometrize
{

class ImageRunner::ImageRunnerImpl
{
public:
    ImageRunnerImpl(const geometrize::Bitmap& targetBitmap) : m_model{targetBitmap, geometrize::commonutil::getAverageImageColor(targetBitmap)} {}
    ImageRunnerImpl(const geometrize::Bitmap& targetBitmap, const geometrize::Bitmap& initialBitmap) : m_model{targetBitmap, initialBitmap} {}
    ~ImageRunnerImpl() = default;
    ImageRunnerImpl& operator=(const ImageRunnerImpl&) = delete;
    ImageRunnerImpl(const ImageRunnerImpl&) = delete;

    std::vector<geometrize::ShapeResult> step(const geometrize::ImageRunnerOptions& options)
    {
        m_model.setSeed(options.seed);
        return m_model.step(options.shapeTypes, options.alpha, options.shapeCount, options.maxShapeMutations, options.maxThreads);
    }

    geometrize::Bitmap& getCurrent()
    {
        return m_model.getCurrent();
    }

    geometrize::Bitmap& getTarget()
    {
        return m_model.getTarget();
    }

    geometrize::Model& getModel()
    {
        return m_model;
    }

private:
    geometrize::Model m_model; ///< The model for the primitive optimization/fitting algorithm.
};

ImageRunner::ImageRunner(const geometrize::Bitmap& targetBitmap) :
    d{std::unique_ptr<ImageRunner::ImageRunnerImpl>(new ImageRunner::ImageRunnerImpl(targetBitmap))}
{}

ImageRunner::ImageRunner(const geometrize::Bitmap& targetBitmap,  const geometrize::Bitmap& initialBitmap) :
    d{std::unique_ptr<ImageRunner::ImageRunnerImpl>(new ImageRunner::ImageRunnerImpl(targetBitmap, initialBitmap))}
{}

ImageRunner::~ImageRunner()
{}

std::vector<geometrize::ShapeResult> ImageRunner::step(const geometrize::ImageRunnerOptions& options)
{
    return d->step(options);
}

geometrize::Bitmap& ImageRunner::getCurrent()
{
    return d->getCurrent();
}

geometrize::Bitmap& ImageRunner::getTarget()
{
    return d->getTarget();
}

geometrize::Model& ImageRunner::getModel()
{
    return d->getModel();
}

}
