#ifndef SOURCE_H
#define SOURCE_H
#include "basiccomponent.h"
class CircuitSimulation;
enum SOURCEMODE{DC,SIN,SQUARE};
class Source : public BasicComponent
{
protected:
    double amplitude=0;
    double freq=0;
    double phase=0;
    double DCvalue=0;
    double Von=0;
    double Vinitial=0;
    double current;

    static QSet<unsigned> index_list;
public:
    SOURCEMODE currentMode=DC;
    Source(unsigned int INDEX);
    QRectF boundingRect() const;
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget);
    friend class CircuitSimulation;
    ~Source();
    void rotate();
    void moveTo(const QPointF scenePoint);
    void getdata(QString &getamp,QString &getfreq,QString &getphase,QString &getDCValue,QString &getVon,QString &getVinitial);
    void setdata(QString getamp,QString getfreq,QString getphase,QString getDCValue,QString getVon,QString getVinitial);
};

#endif // SOURCE_H
