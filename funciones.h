#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SECTORES 5
#define DIAS_HISTORICOS 30
#define LONGITUD_NOMBRE 50

// Limite base OMS (referencia absoluta minima)
#define OMS_LIMITE_PM25 25.0

// Estructura para datos climaticos y contaminantes
typedef struct {
    float pm25;
    float co2;          
    float temperatura;
    float velocidad_viento;
    float humedad;
} Medicion;

// Estructura principal del SECTOR
typedef struct {
    int id;
    char nombre[LONGITUD_NOMBRE];
    Medicion historial[DIAS_HISTORICOS]; 
    Medicion actual;                     
    float prediccion_futura;             
    float maximo_historico;              
    char nivel_contaminacion[20];        
    char recomendacion[300];             
    int alerta_activa;                   
} Sector;


// Utilidades y Validaciones
void limpiarBuffer();
void presionarEnter();
int esDigito(char c); 
int leerEntero(const char* mensaje, int min, int max);
float leerFlotante(const char* mensaje, float min, float max);

// Logica del Negocio (Sectores Quito)
void inicializarSectoresQuito(Sector sectores[]);
void ingresarMedicionActual(Sector* s);
void calcularPrediccionPonderada(Sector* s);
void evaluarAlertasDinamicas(Sector* s);
void generarRecomendaciones(Sector* s);

// Persistencia Individual (.data) y Reportes
void guardarDatosIndividuales(Sector sectores[]);
void cargarDatosIndividuales(Sector sectores[]);
void generarReporteTexto(Sector sectores[]);

#endif