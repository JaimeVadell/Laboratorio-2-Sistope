//Cabeceras de funciones.c

void imprimirNormalizado(double * arregloEnergiaParticulas, int posicionEnergiaMaximaActual, int N, int * arreglolineasProcesadasHijo, int num_children);
void imprimirEnOrden(double * arregloEnergiaParticulas, int posicionEnergiaMaximaActual, int N, const char *nombreArchivoSalida);
int obtenerPosicionMaximaEnergia(double *arregloEnergiaParticulas, int N);
double* CrearArregloEnergiaParticulas(int N);
void obtenerPosicionEnergiaMaxima(double * arregloEnergiaParticulas, int* posicionEnergiaMaximaActual, int posicionEnergiaComparacion);
void sumarEnergiaParticulas(double *arregloEnergiaParticulas, int N, int posicionImpactoParticula, int energiaParticula, int * posicionEnergiaMaximaActual);
int generarNumeroAleatorio(int min, int max);