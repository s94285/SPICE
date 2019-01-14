#include "linearcomponent.h"

LinearComponent::LinearComponent()
{

}

QDataStream &operator<<(QDataStream &qs, const LinearComponent &l)
{
    qs << static_cast<const BasicComponent&>(l);
    return qs;
}

QDataStream &operator>>(QDataStream &qs, LinearComponent &l)
{
    qs >> static_cast<BasicComponent&>(l);
    return qs;
}
