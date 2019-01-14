#include "circuitsimulation.h"
#include <QDebug>
#include <ui_mainwindow.h>
#include "workspace.h"
#include <complex>
#include "complexmatrix.h"
#include <QDialog>
#include <QFormLayout>
#include <QInputDialog>
#include <QLabel>
#include "scope.h"
#include "ui_scope.h"
#include <QMessageBox>
#include <QFileDialog>
CircuitSimulation::CircuitSimulation()
{
    mainWindow=new MainWindow;
    scopeWindow =new Scope(mainWindow);
    scopeWindow->setStyleSheet("background-color: white;");
    //connecting buttons
    connect((mainWindow->toolBarButton)[0],SIGNAL(clicked()),this,SLOT(newFile()));
    connect((mainWindow->toolBarButton)[1],SIGNAL(clicked()),this,SLOT(openFile()));
    connect((mainWindow->toolBarButton)[2],SIGNAL(clicked()),this,SLOT(saveFile()));
    connect((mainWindow->toolBarButton)[3],SIGNAL(clicked()),this,SLOT(run()));
    connect((mainWindow->toolBarButton)[4],SIGNAL(clicked()),this,SLOT(drawLine()));
    connect((mainWindow->toolBarButton)[5],SIGNAL(clicked()),this,SLOT(addGround()));
    connect((mainWindow->toolBarButton)[6],SIGNAL(clicked()),this,SLOT(addResistor()));
    connect((mainWindow->toolBarButton)[7],SIGNAL(clicked()),this,SLOT(addCapacitor()));
    connect((mainWindow->toolBarButton)[8],SIGNAL(clicked()),this,SLOT(addInductor()));
    connect((mainWindow->toolBarButton)[9],SIGNAL(clicked()),this,SLOT(cut()));
    connect((mainWindow->toolBarButton)[10],SIGNAL(clicked()),this,SLOT(move()));
    connect((mainWindow->toolBarButton)[11],SIGNAL(clicked()),this,SLOT(addSource()));


    mainWindow->show();
    workspace = mainWindow->ui->workspace;
}

CircuitSimulation::~CircuitSimulation(){
    for(auto i:components)
        delete i;   //clean up all components
    delete mainWindow;
}

void CircuitSimulation::init(){
    workspace->currentMode=IDLE;
    workspace->setComponents(components);
    workspace->setNodes(nodes);
    workspace->setLines(lines);
    workspace->drawComponents();
    workspace->setSimulator(this);
    scopeWindow->ui->scopeView->points=&points;
}

void CircuitSimulation::initRun()
{
    scopeWindow->ui->scopeView->nodes.clear();
    scopeWindow->ui->scopeView->linearComponents.clear();
    scopeWindow->ui->scopeView->sources.clear();
    scopeWindow->ui->scopeView->clear();
}

void CircuitSimulation::drawFunction(Node *aNode)
{
    if(scopeWindow->ui->scopeView->nodes.indexOf(aNode)>-1)return;
    scopeWindow->ui->scopeView->nodes.append(aNode);
    scopeWindow->ui->scopeView->clear();
    scopeWindow->ui->scopeView->draw();
}

void CircuitSimulation::drawFunction(LinearComponent *aLC)
{
    if(scopeWindow->ui->scopeView->linearComponents.indexOf(aLC)>-1)return;
    scopeWindow->ui->scopeView->linearComponents.append(aLC);
    scopeWindow->ui->scopeView->clear();
    scopeWindow->ui->scopeView->draw();
}

void CircuitSimulation::drawFunction(Source *as)
{
    if(scopeWindow->ui->scopeView->sources.indexOf(as)>-1)return;
    scopeWindow->ui->scopeView->sources.append(as);
    scopeWindow->ui->scopeView->clear();
    scopeWindow->ui->scopeView->draw();
}

void CircuitSimulation::run(){
    using namespace  std;
    qDebug()<<"Run\n";
    bool ok;
    double runTime=QInputDialog::getDouble(nullptr,"Input run time","Run time:",0,0,100000,3,&ok);
    if(abs(runTime)<1e-9&&ok)
    {
        QMessageBox::critical(nullptr,tr("Error"),tr("Please check your runtime"));
    }
    if(!ok||abs(runTime)<1e-9)return;
    try{
        int gndIndex=-1;
        QVector<Source*> sources;
        QVector<LinearComponent*> linearComponents;


        for(auto i:components){
            if(dynamic_cast<Source*>(i)){
                sources.append(dynamic_cast<Source*>(i));
            }else if(dynamic_cast<LinearComponent*>(i)){
                linearComponents.append(dynamic_cast<LinearComponent*>(i));
            }
        }
        for(int sourceIndex=0;sourceIndex<sources.size();sourceIndex++){
            Node *n1=nullptr,*n2=nullptr;
            for(Node *i:nodes)
                if(i->connectedPorts.indexOf(qMakePair(static_cast<BasicComponent*>(sources[sourceIndex]),&sources[sourceIndex]->ports[0]))>-1)
                    n1=i;
            for(Node *i:nodes)
                if(i->connectedPorts.indexOf(qMakePair(static_cast<BasicComponent*>(sources[sourceIndex]),&sources[sourceIndex]->ports[1]))>-1)
                    n2=i;
            if(((!n1)&&n2)||(n1&&(!n2)))
                    throw ("source");
            else if(!(n1&&n2))
            {
                sources.erase(sources.begin()+sourceIndex);
                sourceIndex--;
                qDebug()<<" source bug"<<endl;
            }
        }
        for(LinearComponent *rlc:linearComponents){
            Node *n1=nullptr,*n2=nullptr;
            for(Node* n:nodes){
                if(n->connectedPorts.indexOf(qMakePair(dynamic_cast<BasicComponent*>(rlc),&rlc->ports[0]))>-1)
                    n1=n;
                else if(n->connectedPorts.indexOf(qMakePair(dynamic_cast<BasicComponent*>(rlc),&rlc->ports[1]))>-1)
                    n2=n;
            }
            if(((!n1)&&n2)||(n1&&(!n2)))
                    throw 0.1 ;

        }
        //clean old value
        for(auto node:nodes)node->voltage.clear();
        for(auto source:sources)source->current.clear();
        for(auto lc:linearComponents)lc->current.clear();
        int matrix_size=nodes.size()+sources.size();
        double angularFrequency;
        for(int currentSourceIndex=0;currentSourceIndex<sources.size();currentSourceIndex++){
            if(sources[currentSourceIndex]->currentMode==SIN){
                angularFrequency=sources[currentSourceIndex]->freq*2*M_PI;
                complex<double> **matrix=new complex<double>*[matrix_size];
                complex<double> *constVector=new complex<double>[matrix_size];
                for(int i=0;i<matrix_size;i++){
                    matrix[i]=new complex <double>[matrix_size];
                    for(int j=0;j<matrix_size;j++)
                        matrix[i][j]=0;
                }
                for(int i=0;gndIndex==-1&&i<nodes.size();i++)
                {
                    for(int j=0;gndIndex==-1&&j<nodes[i]->connectedPorts.size();j++)
                    {
                        if(dynamic_cast<ground*>(nodes[i]->connectedPorts[j].first))
                        {
                            gndIndex=i;
                        }
                    }
                }
                if(gndIndex==-1)throw 1;

                for(int i=0;i<nodes.size();i++){    //for nodal analysis
                    if(i==gndIndex){
                        matrix[i][i]=1;
                        constVector[i]=0;
                        continue;
                    }
                    //TODO:write m[i][i]
                    for(auto &port:nodes[i]->connectedPorts){
                        BasicComponent *bc=port.first;
                        Source *sc=dynamic_cast<Source*>(bc);
                        LinearComponent *lc=dynamic_cast<LinearComponent*>(bc);
                        if(sc){
                            if(sc->ports.indexOf(*port.second)==0)
                                matrix[i][nodes.size()+sources.indexOf(sc)]=1;
                            else
                                matrix[i][nodes.size()+sources.indexOf(sc)]=-1;
                        }else if(lc){
                            matrix[i][i]+=1./lc->getimpedance(angularFrequency);
                            QPoint *adjacentPort;
                            for(auto &pt:lc->ports)
                                if(&pt!=port.second)
                                    adjacentPort=&pt;
                            int adjacentNodeIndex;
                            for(adjacentNodeIndex=0;adjacentNodeIndex<nodes.size();adjacentNodeIndex++){
                                if(adjacentNodeIndex==i)continue;   //skip self node
                                if(nodes[adjacentNodeIndex]->connectedPorts.indexOf(qMakePair(bc,adjacentPort))>-1){
                                    break;
                                }
                            }
                            if(adjacentNodeIndex>=nodes.size())qDebug() << "adj error";
                            matrix[i][adjacentNodeIndex]+=-1./lc->getimpedance(angularFrequency);
                        }
                    }
                }
                //additional imformation from voltage source
                for(int sourceIndex=0;sourceIndex<sources.size();sourceIndex++){
                    Node *n1,*n2;
                    for(Node *i:nodes)
                        if(i->connectedPorts.indexOf(qMakePair(static_cast<BasicComponent*>(sources[sourceIndex]),&sources[sourceIndex]->ports[0]))>-1)
                            n1=i;
                    for(Node *i:nodes)
                        if(i->connectedPorts.indexOf(qMakePair(static_cast<BasicComponent*>(sources[sourceIndex]),&sources[sourceIndex]->ports[1]))>-1)
                            n2=i;
                    matrix[nodes.size()+sourceIndex][nodes.indexOf(n1)]=1;
                    matrix[nodes.size()+sourceIndex][nodes.indexOf(n2)]=-1;
                    constVector[nodes.size()+sourceIndex]=(sourceIndex==currentSourceIndex)?sources[sourceIndex]->getACVoltage():0;
                }
                //calculate result
                complex<double> **inverse=new complex<double>*[matrix_size];
                for(int i=0;i<matrix_size;i++){
                    inverse[i]=new complex<double>[matrix_size];
                }
                ComplexMatrix::getInverse(matrix,inverse,matrix_size);
                complex<double> *result=new complex<double>[matrix_size];
                for(int i=0;i<matrix_size;i++){
                    result[i]=0;
                    for(int j=0;j<matrix_size;j++)
                        result[i]+=inverse[i][j]*constVector[j];
                }
                //write value
                for(int i=0;i<matrix_size;i++)
                    qDebug() << result[i].real() << "," << result[i].imag() << " ";
                for(int i=0;i<nodes.size();i++)
                    nodes[i]->voltage.append(qMakePair(result[i],angularFrequency));
                for(int i=0;i<sources.size();i++)
                    sources[i]->current.append(qMakePair(result[nodes.size()+i],angularFrequency));
                for(LinearComponent *rlc:linearComponents){
                    Node *n1=nullptr,*n2=nullptr;
                    for(Node* n:nodes){
                        if(n->connectedPorts.indexOf(qMakePair(dynamic_cast<BasicComponent*>(rlc),&rlc->ports[0]))>-1)
                            n1=n;
                        else if(n->connectedPorts.indexOf(qMakePair(dynamic_cast<BasicComponent*>(rlc),&rlc->ports[1]))>-1)
                            n2=n;
                    }
                    if(n1&&n2)
                        rlc->current.append(qMakePair((n1->voltage.last().first-n2->voltage.last().first)/rlc->getimpedance(n1->voltage.last().second),n1->voltage.last().second));
                }
            }else if(sources[currentSourceIndex]->currentMode==SQUARE){
                double baseAngularFrequency=sources[currentSourceIndex]->freq*2*M_PI;  //base freq
                double oldAmplitude=sources[currentSourceIndex]->amplitude;     //backup
                double oldPhase=sources[currentSourceIndex]->phase;
                sources[currentSourceIndex]->phase=0;
                for(int k=1;k<1000;k+=2){     //square is composited by odd harmonics
                    angularFrequency=baseAngularFrequency*k;
                    sources[currentSourceIndex]->amplitude=2*(sources[currentSourceIndex]->Von-sources[currentSourceIndex]->Vinitial)/(M_PI*k);
                    complex<double> **matrix=new complex<double>*[matrix_size];
                    complex<double> *constVector=new complex<double>[matrix_size];
                    for(int i=0;i<matrix_size;i++){
                        matrix[i]=new complex <double>[matrix_size];
                        for(int j=0;j<matrix_size;j++)
                            matrix[i][j]=0;
                    }
                    for(int i=0;gndIndex==-1&&i<nodes.size();i++)
                    {
                        for(int j=0;gndIndex==-1&&j<nodes[i]->connectedPorts.size();j++)
                        {
                            if(dynamic_cast<ground*>(nodes[i]->connectedPorts[j].first))
                            {
                                gndIndex=i;
                            }
                        }
                    }
                    if(gndIndex==-1)throw 1;

                    for(int i=0;i<nodes.size();i++){    //for nodal analysis
                        if(i==gndIndex){
                            matrix[i][i]=1;
                            constVector[i]=0;
                            continue;
                        }
                        //TODO:write m[i][i]
                        for(auto &port:nodes[i]->connectedPorts){
                            BasicComponent *bc=port.first;
                            Source *sc=dynamic_cast<Source*>(bc);
                            LinearComponent *lc=dynamic_cast<LinearComponent*>(bc);
                            if(sc){
                                if(sc->ports.indexOf(*port.second)==0)
                                    matrix[i][nodes.size()+sources.indexOf(sc)]=1;
                                else
                                    matrix[i][nodes.size()+sources.indexOf(sc)]=-1;
                            }else if(lc){
                                matrix[i][i]+=1./lc->getimpedance(angularFrequency);
                                QPoint *adjacentPort;
                                for(auto &pt:lc->ports)
                                    if(&pt!=port.second)
                                        adjacentPort=&pt;
                                int adjacentNodeIndex;
                                for(adjacentNodeIndex=0;adjacentNodeIndex<nodes.size();adjacentNodeIndex++){
                                    if(adjacentNodeIndex==i)continue;   //skip self node
                                    if(nodes[adjacentNodeIndex]->connectedPorts.indexOf(qMakePair(bc,adjacentPort))>-1){
                                        break;
                                    }
                                }
                                if(adjacentNodeIndex>=nodes.size())qDebug() << "adj error";
                                matrix[i][adjacentNodeIndex]+=-1./lc->getimpedance(angularFrequency);
                            }
                        }
                    }
                    //additional imformation from voltage source
                    for(int sourceIndex=0;sourceIndex<sources.size();sourceIndex++){
                        Node *n1,*n2;
                        for(Node *i:nodes)
                            if(i->connectedPorts.indexOf(qMakePair(static_cast<BasicComponent*>(sources[sourceIndex]),&sources[sourceIndex]->ports[0]))>-1)
                                n1=i;
                        for(Node *i:nodes)
                            if(i->connectedPorts.indexOf(qMakePair(static_cast<BasicComponent*>(sources[sourceIndex]),&sources[sourceIndex]->ports[1]))>-1)
                                n2=i;
                        matrix[nodes.size()+sourceIndex][nodes.indexOf(n1)]=1;
                        matrix[nodes.size()+sourceIndex][nodes.indexOf(n2)]=-1;
                        constVector[nodes.size()+sourceIndex]=(sourceIndex==currentSourceIndex)?sources[sourceIndex]->getACVoltage():0;
                    }
                    //calculate result
                    complex<double> **inverse=new complex<double>*[matrix_size];
                    for(int i=0;i<matrix_size;i++){
                        inverse[i]=new complex<double>[matrix_size];
                    }
                    ComplexMatrix::getInverse(matrix,inverse,matrix_size);
                    complex<double> *result=new complex<double>[matrix_size];
                    for(int i=0;i<matrix_size;i++){
                        result[i]=0;
                        for(int j=0;j<matrix_size;j++)
                            result[i]+=inverse[i][j]*constVector[j];
                    }
                    for(int i=0;i<matrix_size;i++)qDebug() << result[i].real() << " , " << result[i].imag();
                    //write value
                    for(int i=0;i<nodes.size();i++)
                        nodes[i]->voltage.append(qMakePair(result[i],angularFrequency));
                    for(int i=0;i<sources.size();i++)
                        sources[i]->current.append(qMakePair(result[nodes.size()+i],angularFrequency));
                    for(LinearComponent *rlc:linearComponents){
                        Node *n1=nullptr,*n2=nullptr;
                        for(Node* n:nodes){
                            if(n->connectedPorts.indexOf(qMakePair(dynamic_cast<BasicComponent*>(rlc),&rlc->ports[0]))>-1)
                                n1=n;
                            else if(n->connectedPorts.indexOf(qMakePair(dynamic_cast<BasicComponent*>(rlc),&rlc->ports[1]))>-1)
                                n2=n;
                        }
                        if(n1&&n2)
                            rlc->current.append(qMakePair((n1->voltage.last().first-n2->voltage.last().first)/rlc->getimpedance(n1->voltage.last().second),n1->voltage.last().second));
                    }
                }
                sources[currentSourceIndex]->amplitude=oldAmplitude;
                sources[currentSourceIndex]->phase=oldPhase;
            }else if(sources[currentSourceIndex]->currentMode==DC){
                double baseAngularFrequency=M_PI*2./1e6;  //base freq with large time
                double oldAmplitude=sources[currentSourceIndex]->amplitude;     //backup
                double oldPhase=sources[currentSourceIndex]->phase;
                sources[currentSourceIndex]->phase=90;  //cosine
                qDebug() << "DC is: " << sources[currentSourceIndex]->DCvalue;
                for(int k=1;k<1000;k++){     //DC
                    angularFrequency=baseAngularFrequency*k;
                    sources[currentSourceIndex]->amplitude=4.0*sources[currentSourceIndex]->DCvalue*sin(M_PI*k/2)/(M_PI*k);
                    complex<double> **matrix=new complex<double>*[matrix_size];
                    complex<double> *constVector=new complex<double>[matrix_size];
                    for(int i=0;i<matrix_size;i++){
                        matrix[i]=new complex <double>[matrix_size];
                        for(int j=0;j<matrix_size;j++)
                            matrix[i][j]=0;
                    }
                    for(int i=0;gndIndex==-1&&i<nodes.size();i++)
                    {
                        for(int j=0;gndIndex==-1&&j<nodes[i]->connectedPorts.size();j++)
                        {
                            if(dynamic_cast<ground*>(nodes[i]->connectedPorts[j].first))
                            {
                                gndIndex=i;
                            }
                        }
                    }
                    if(gndIndex==-1)throw 1;

                    for(int i=0;i<nodes.size();i++){    //for nodal analysis
                        if(i==gndIndex){
                            matrix[i][i]=1;
                            constVector[i]=0;
                            continue;
                        }
                        //TODO:write m[i][i]
                        for(auto &port:nodes[i]->connectedPorts){
                            BasicComponent *bc=port.first;
                            Source *sc=dynamic_cast<Source*>(bc);
                            LinearComponent *lc=dynamic_cast<LinearComponent*>(bc);
                            if(sc){
                                if(sc->ports.indexOf(*port.second)==0)
                                    matrix[i][nodes.size()+sources.indexOf(sc)]=1;
                                else
                                    matrix[i][nodes.size()+sources.indexOf(sc)]=-1;
                            }else if(lc){
                                matrix[i][i]+=1./lc->getimpedance(angularFrequency);
                                QPoint *adjacentPort;
                                for(auto &pt:lc->ports)
                                    if(&pt!=port.second)
                                        adjacentPort=&pt;
                                int adjacentNodeIndex;
                                for(adjacentNodeIndex=0;adjacentNodeIndex<nodes.size();adjacentNodeIndex++){
                                    if(adjacentNodeIndex==i)continue;   //skip self node
                                    if(nodes[adjacentNodeIndex]->connectedPorts.indexOf(qMakePair(bc,adjacentPort))>-1){
                                        break;
                                    }
                                }
                                if(adjacentNodeIndex>=nodes.size())qDebug() << "adj error";
                                matrix[i][adjacentNodeIndex]+=-1./lc->getimpedance(angularFrequency);
                            }
                        }
                    }
                    //additional imformation from voltage source
                    for(int sourceIndex=0;sourceIndex<sources.size();sourceIndex++){
                        Node *n1,*n2;
                        for(Node *i:nodes)
                            if(i->connectedPorts.indexOf(qMakePair(static_cast<BasicComponent*>(sources[sourceIndex]),&sources[sourceIndex]->ports[0]))>-1)
                                n1=i;
                        for(Node *i:nodes)
                            if(i->connectedPorts.indexOf(qMakePair(static_cast<BasicComponent*>(sources[sourceIndex]),&sources[sourceIndex]->ports[1]))>-1)
                                n2=i;
                        matrix[nodes.size()+sourceIndex][nodes.indexOf(n1)]=1;
                        matrix[nodes.size()+sourceIndex][nodes.indexOf(n2)]=-1;
                        constVector[nodes.size()+sourceIndex]=(sourceIndex==currentSourceIndex)?sources[sourceIndex]->getACVoltage():0;
                    }
                    //calculate result
                    complex<double> **inverse=new complex<double>*[matrix_size];
                    for(int i=0;i<matrix_size;i++){
                        inverse[i]=new complex<double>[matrix_size];
                    }
                    ComplexMatrix::getInverse(matrix,inverse,matrix_size);
                    complex<double> *result=new complex<double>[matrix_size];
                    for(int i=0;i<matrix_size;i++){
                        result[i]=0;
                        for(int j=0;j<matrix_size;j++)
                            result[i]+=inverse[i][j]*constVector[j];
                    }
                    for(int i=0;i<matrix_size;i++)qDebug() << result[i].real() << " , " << result[i].imag();
                    //write value
                    for(int i=0;i<nodes.size();i++)
                        nodes[i]->voltage.append(qMakePair(result[i],angularFrequency));
                    for(int i=0;i<sources.size();i++)
                        sources[i]->current.append(qMakePair(result[nodes.size()+i],angularFrequency));
                    for(LinearComponent *rlc:linearComponents){
                        Node *n1=nullptr,*n2=nullptr;
                        for(Node* n:nodes){
                            if(n->connectedPorts.indexOf(qMakePair(dynamic_cast<BasicComponent*>(rlc),&rlc->ports[0]))>-1)
                                n1=n;
                            else if(n->connectedPorts.indexOf(qMakePair(dynamic_cast<BasicComponent*>(rlc),&rlc->ports[1]))>-1)
                                n2=n;
                        }
                        if(n1&&n2)
                            rlc->current.append(qMakePair((n1->voltage.last().first-n2->voltage.last().first)/rlc->getimpedance(n1->voltage.last().second),n1->voltage.last().second));
                    }
                }
                sources[currentSourceIndex]->amplitude=oldAmplitude;
                sources[currentSourceIndex]->phase=oldPhase;
            }
        }
    }catch(int){
        QMessageBox::critical(nullptr,"Error occurs","Please check your ground");
        return;
    }
    catch(const char){
        QMessageBox::critical(nullptr,"Error occurs","Please check your source connection");
        return;
    }
    catch(double)
    {
        QMessageBox::critical(nullptr,"Error occurs","Please check your components connection");
        return;
    }catch(...){
        QMessageBox::critical(nullptr,"Error occurs","Unexpected error");
        return;
    }
    //evaluate points
    for(auto i:points)delete points[i];
    points.clear();
    double amp,phase,angularFreq,timescale=runTime/scale;
    for(int i=0;i<nodes.size();i++)
    {
        points[nodes[i]]=new double[scale];
        for(int k=0;k<scale;k++)points[nodes[i]][k]=0;
        for(int j=0;j<nodes[i]->voltage.size();j++){
            amp=abs(nodes[i]->voltage[j].first);
            phase=arg(nodes[i]->voltage[j].first);
            angularFreq=nodes[i]->voltage[j].second;
            for(int ti=0;ti<scale;ti++){
                double value=amp*sin(angularFreq*ti*timescale+phase);
                points[nodes[i]][ti]+=value;
            }
        }
    }
    for(auto i:components){
        LinearComponent *lc=dynamic_cast<LinearComponent*>(i);
        Source *sc=dynamic_cast<Source*>(i);
        if(lc){
            points[lc]=new double[scale];
            for(int k=0;k<scale;k++)points[lc][k]=0;
            for(int j=0;j<lc->current.size();j++){
                amp=abs(lc->current[j].first);
                phase=arg(lc->current[j].first);
                angularFreq=lc->current[j].second;
                for(int ti=0;ti<scale;ti++){
                    double value=amp*sin(angularFreq*ti*timescale+phase);
                    points[lc][ti]+=value;
                }
            }
        }
        if(sc){
            points[sc]=new double[scale];
            for(int k=0;k<scale;k++)points[sc][k]=0;
            for(int j=0;j<sc->current.size();j++){
                amp=abs(sc->current[j].first);
                phase=arg(sc->current[j].first);
                angularFreq=sc->current[j].second;
                for(int ti=0;ti<scale;ti++){
                    double value=amp*sin(angularFreq*ti*timescale+phase);
                    points[sc][ti]+=value;
                }
            }
        }
    }
    scopeWindow->ui->scopeView->runTime=runTime;
    initRun();
    scopeWindow->show();
    scopeWindow->raise();
    scopeWindow->activateWindow();
    workspace->currentMode=RUN;
}
void CircuitSimulation::drawLine(){
    qDebug()<<"drawLine\n";
    workspace->currentMode=DRAWLINE;
}
void CircuitSimulation::addGround(){
    qDebug()<<"addGround\n";
    workspace->currentMode=MOVE;
    unsigned int ind=0;
    for(;;ind++)
        if(ground::index_list.find(ind)==ground::index_list.end())
            break;
    ground *g1=new ground(ind);
    ground::index_list.insert(ind);
    components.append(g1);
    workspace->itemSelected=g1;
    workspace->drawComponents();
}
void CircuitSimulation::addResistor(){
    qDebug()<<"addResistor\n";
    workspace->currentMode=MOVE;
    unsigned int ind=0;
    for(;;ind++)
        if(Resistor::index_list.find(ind)==Resistor::index_list.end())
            break;
    Resistor *r1=new Resistor(ind);
    Resistor::index_list.insert(ind);
    components.append(r1);
    workspace->itemSelected=r1;
    workspace->drawComponents();
}
void CircuitSimulation::addCapacitor(){
    qDebug()<<"addCapacitor\n";
    workspace->currentMode=MOVE;
    unsigned int ind=0;
    for(;;ind++)
        if(Capacitor::index_list.find(ind)==Capacitor::index_list.end())
            break;
    Capacitor *c1=new Capacitor(ind);
    Capacitor::index_list.insert(ind);
    components.append(c1);
    workspace->itemSelected=c1;
    workspace->drawComponents();
}
void CircuitSimulation::addInductor(){
    qDebug()<<"addInductor\n";
    workspace->currentMode=MOVE;
    unsigned int ind=0;
    for(;;ind++)
        if(Inductor::index_list.find(ind)==Inductor::index_list.end())
            break;
    Inductor *i1=new Inductor(ind);
    Inductor::index_list.insert(ind);
    components.append(i1);
    workspace->itemSelected=i1;
    workspace->drawComponents();
}
void CircuitSimulation::cut(){
    qDebug()<<"cut\n";
    workspace->currentMode=CUT;
}
void CircuitSimulation::move(){
    qDebug()<<"move\n";
    workspace->currentMode=MOVE;
}

void CircuitSimulation::addSource()
{
    qDebug()<<"addSource\n";
    workspace->currentMode=MOVE;
    unsigned int ind=0;
    for(;;ind++)
        if(Source::index_list.find(ind)==Source::index_list.end())
            break;
    Source *s1=new Source(ind);
    Source::index_list.insert(ind);
    components.append(s1);
    workspace->itemSelected=s1;
    workspace->drawComponents();
}

struct ItemCount{
    int resistors,capacitors,inductors,grounds,sources,nodes;
};
QDataStream &operator>>(QDataStream& qs,ItemCount& ic){     //input
    qs >> ic.resistors >> ic.capacitors >> ic.inductors >> ic.grounds >> ic.sources >> ic.nodes;
    return qs;
}
QDataStream &operator<<(QDataStream& qs,const ItemCount& ic){     //output
    qs << ic.resistors << ic.capacitors << ic.inductors << ic.grounds << ic.sources << ic.nodes;
    return qs;
}

void CircuitSimulation::openFile()
{
    QString f_str = QFileDialog::getOpenFileName(nullptr, "Open","C:\\", "Schematics (*.qasc)");
    qDebug() << f_str;
    QFile openFile(f_str);
    if(!openFile.open(QIODevice::ReadOnly)){
        qDebug() << "Cannot open file: " << f_str;
        return;
    }
    initRun();
    scopeWindow->close();
    for(auto i:components)delete i;
    components.clear();
    for(auto nodeOfLineToDelete:nodes){
        for(QGraphicsItem *line:nodeOfLineToDelete->childItems()){
            lines.removeOne(dynamic_cast<Line*>(line));
            delete line;
        }
        delete nodeOfLineToDelete;
    }
    lines.clear();
    nodes.clear();
    QDataStream openStream(&openFile);
    ItemCount itemCount;
    openStream >> itemCount;
    for(int i=0;i<itemCount.resistors;i++){
        Resistor* resistor = new Resistor(0);
        openStream >> (*resistor);
        components.append(resistor);
    }
    for(int i=0;i<itemCount.capacitors;i++){
        Capacitor* capacitor = new Capacitor(0);
        openStream >> (*capacitor);
        components.append(capacitor);
    }
    for(int i=0;i<itemCount.inductors;i++){
        Inductor* inductor = new Inductor(0);
        openStream >> (*inductor);
        components.append(inductor);
    }
    for(int i=0;i<itemCount.grounds;i++){
        ground* groundd = new ground(0);
        openStream >> (*groundd);
        components.append(groundd);
    }
    for(int i=0;i<itemCount.sources;i++){
        Source* source = new Source(0);
        openStream >> (*source);
        components.append(source);
    }
    for(int i=0;i<itemCount.nodes;i++){
        QVector<QPair<int,QPoint>> connectedPorts;
        QVector<Line> linesOfNode;
        int sizeOfLines;
        openStream >> connectedPorts >> sizeOfLines;

        Node* node = new Node();
        for(int j=0;j<sizeOfLines;j++){
            Line *newLine = new Line(QPoint(0,0),QPoint(0,0));
            openStream >> (*newLine);
            lines.append(newLine);
            node->addToGroup(newLine);
            newLine->setGroup(node);
        }
        for(auto k:connectedPorts){
            node->connectedPorts.append(qMakePair(components[k.first],&(components[k.first]->ports[components[k.first]->ports.indexOf(k.second)])));
        }
        nodes.append(node);
    }
    workspace->drawComponents();
}

void CircuitSimulation::saveFile()
{
    QString f_str = QFileDialog::getSaveFileName(nullptr, "Save","C:\\", "Schematics (*.qasc)");
    qDebug() << f_str;
    QFile saveFile(f_str);

    if(!saveFile.open(QIODevice::WriteOnly)){
        qDebug() << "Cannot save file: " << f_str;
        return;
    }
    QDataStream saveStream(&saveFile);
    QVector<Resistor*> resistor_ptr;
    QVector<Capacitor*> capacitor_ptr;
    QVector<Inductor*> inductor_ptr;
    QVector<ground*> ground_ptr;
    QVector<Source*> source_ptr;
    QVector<BasicComponent*> basicComponent_ptr;    //for finding index
    for(auto i:components){
        Resistor* resistor=dynamic_cast<Resistor*>(i);
        Capacitor* capacitor=dynamic_cast<Capacitor*>(i);
        Inductor* inductor=dynamic_cast<Inductor*>(i);
        ground* groundd=dynamic_cast<ground*>(i);
        Source* source=dynamic_cast<Source*>(i);
        if(resistor)
            resistor_ptr.append(resistor);
        if(capacitor)
            capacitor_ptr.append(capacitor);
        if(inductor)
            inductor_ptr.append(inductor);
        if(groundd)
            ground_ptr.append(groundd);
        if(source)
            source_ptr.append(source);
    }
    std::copy(resistor_ptr.begin(),resistor_ptr.end(),std::back_inserter(basicComponent_ptr));
    std::copy(capacitor_ptr.begin(),capacitor_ptr.end(),std::back_inserter(basicComponent_ptr));
    std::copy(inductor_ptr.begin(),inductor_ptr.end(),std::back_inserter(basicComponent_ptr));
    std::copy(ground_ptr.begin(),ground_ptr.end(),std::back_inserter(basicComponent_ptr));
    std::copy(source_ptr.begin(),source_ptr.end(),std::back_inserter(basicComponent_ptr));
    ItemCount itemCount;
    itemCount.resistors=resistor_ptr.size();
    itemCount.capacitors=capacitor_ptr.size();
    itemCount.inductors=inductor_ptr.size();
    itemCount.grounds=ground_ptr.size();
    itemCount.sources=source_ptr.size();
    itemCount.nodes=nodes.size();
    saveStream << itemCount;
    for(auto i:resistor_ptr)saveStream << (*i);
    for(auto i:capacitor_ptr)saveStream << (*i);
    for(auto i:inductor_ptr)saveStream << (*i);
    for(auto i:ground_ptr)saveStream << (*i);
    for(auto i:source_ptr)saveStream << (*i);

    for(auto node:nodes){
        QVector<QPair<int,QPoint>> connectedPorts;  //index of basiccomponent and connected port
        QVector<Line*> linesOfNode;

        for(auto line:lines){
            if(dynamic_cast<Node*>(line->group())==node){
                //l is in group node i
                linesOfNode.append(line);
            }
        }

        for(auto port:node->connectedPorts){
            connectedPorts.append(qMakePair(basicComponent_ptr.indexOf(port.first),*(port.second)));
        }
        int sizeOfLines = linesOfNode.size();
        saveStream << connectedPorts << sizeOfLines;

        for(auto i:linesOfNode){
            saveStream << (*i);
        }
    }
    saveFile.close();
}

void CircuitSimulation::newFile()
{
    initRun();
    scopeWindow->close();
    for(auto i:components)delete i;
    components.clear();
    for(auto nodeOfLineToDelete:nodes){
        for(QGraphicsItem *line:nodeOfLineToDelete->childItems()){
            lines.removeOne(dynamic_cast<Line*>(line));
            delete line;
        }
        delete nodeOfLineToDelete;
    }
    lines.clear();
    nodes.clear();
    workspace->drawComponents();
    workspace->scene->update();
}

QSet<unsigned> Source::index_list;

/* QASC File Format
 * struct itemCount
 * resistors
 * capacitors
 * inductors
 * grounds
 * sources
 * node{
 *      QVector<QPair<int,QPoint>> connectedPorts
 *      int sizeOfLines
 *      linesOfNode
 * }
 */
