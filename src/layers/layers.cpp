#include "midier/layers/layers.h"

namespace midier
{

ILayers::Iterator::Iterator(ILayers & layers, unsigned index) :
    _layers(layers),
    _index(index)
{}

Layer & ILayers::Iterator::operator*()
{
    return _layers[_index];
}

void ILayers::Iterator::operator++()
{
    ++_index;
}

bool ILayers::Iterator::operator!=(const Iterator & other)
{
    return _index != other._index;
}

ILayers::ILayers(Layer layers[], unsigned count) :
    _layers(layers),
    _count(count)
{}

unsigned ILayers::count() const
{
    return _count;
}

unsigned ILayers::running() const
{
    unsigned count = 0;

    for (auto & layer : (ILayers &)*this)
    {
        if (layer.running())
        {
            ++count;
        }
    }

    return count;
}

bool ILayers::idle() const
{
    return running() == 0;
}

Layer & ILayers::operator[](unsigned index)
{
    return _layers[index];
}

ILayers::Iterator ILayers::begin()
{
    return Iterator(*this, 0);
}

ILayers::Iterator ILayers::end()
{
    return Iterator(*this, count());
}

void ILayers::eval(void(*callback)(Layer &))
{
    for (auto & layer : *this)
    {
        if (layer.idle())
        {
            continue;
        }

        callback(layer);
    }
}

bool ILayers::all(bool(*predicate)(const Layer &))
{
    for (const auto & layer : *this)
    {
        if (layer.idle())
        {
            continue;
        }

        if (!predicate(layer))
        {
            return false;
        }
    }

    return true;
}

bool ILayers::any(bool(*predicate)(const Layer &))
{
    for (const auto & layer : *this)
    {
        if (layer.idle())
        {
            continue;
        }

        if (predicate(layer))
        {
            return true;
        }
    }

    return false;
}

bool ILayers::none(bool(*predicate)(const Layer &))
{
    return !any(predicate);
}

void ILayers::record()
{
    eval([](Layer & layer) { layer.record(); });
}

void ILayers::click()
{
    eval([](Layer & layer) { layer.click(); });
}

void ILayers::revoke()
{
    eval([](Layer & layer) { layer.revoke(); });
}

} // midier
