#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

#define MAX_SECTORES 5
#define DIAS_HISTORICOS 30
#define LONGITUD_NOMBRE 50

#define LIMITE_PM25 25.0  
#define LIMITE_SO2 40.0   
#define LIMITE_NO2 25.0   
#define LIMITE_CO2 1000.0 

typedef struct {
    float pm25;         
    float so2;          
    float no2;          
    float co2;          
    float temperatura;
    float velocidad_viento;
    float humedad;
} Medicion;

typedef struct {
    int id;
    char nombre[LONGITUD_NOMBRE];
    
    Medicion historial[DIAS_HISTORICOS]; 
    Medicion actual;                     
    
    float prediccion_futura;             
    float maximo_historico;              
    char nivel_contaminacion[20];      
    
    char rec_social[250];
    char rec_cultural[250];
    char rec_ambiental[250];
    char rec_global[250];
    
    int alerta_activa;
} Sector;

// --- FUNCIONES DEL SISTEMA ---

void limpiarBuffer();
void presionarEnter();
int leerEntero(const char* mensaje, int min, int max);
float leerFlotante(const char* mensaje, float min, float max);

void inicializarSectoresQuito(Sector sectores[]);
void ingresarMedicionActual(Sector* s);
void calcularPrediccionPonderada(Sector* s);
void evaluarAlertasDinamicas(Sector* s);
void generarRecomendaciones(Sector* s);
void verHistorialSector(Sector sectores[]);

void guardarDatosIndividuales(Sector sectores[]);
void cargarDatosIndividuales(Sector sectores[]);
void generarReporteTexto(Sector sectores[]);

#endif