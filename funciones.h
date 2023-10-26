//Cabeceras de funciones.c

void leerArchivo(const char *nombreArchivo, int N, int flagD, const char *nombreArchivoSalida) ;
void imprimirNormalizado(double * arregloEnergiaParticulas, int posicionEnergiaMaximaActual, int N, const char *nombreArchivoSalida);
void imprimirEnOrden(double * arregloEnergiaParticulas, int posicionEnergiaMaximaActual, int N, const char *nombreArchivoSalida);

double* CrearArregloEnergiaParticulas(int N);
void obtenerPosicionEnergiaMaxima(double * arregloEnergiaParticulas, int* posicionEnergiaMaximaActual, int posicionEnergiaComparacion);
void sumarEnergiaParticulas(double *arregloEnergiaParticulas, int N, int posicionImpactoParticula, int energiaParticula, int * posicionEnergiaMaximaActual);
int generarNumeroAleatorio(int min, int max);