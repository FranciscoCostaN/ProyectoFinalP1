#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // Para la fecha y hora en el reporte

// --- CONFIGURACION GENERAL ---
#define MAX_SECTORES 5
#define DIAS_HISTORICOS 30
#define LONGITUD_NOMBRE 50

// --- LIMITES DE NORMATIVA (OMS 2021 / TULSMA) ---
// Usamos estos valores para decidir si se activan las alertas de gases especificos
#define LIMITE_PM25 25.0  
#define LIMITE_SO2 40.0   
#define LIMITE_NO2 25.0   
#define LIMITE_CO2 1000.0 // Referencial para espacios densos

// Estructura para almacenar una medicion completa (Gases + Clima)
typedef struct {
    float pm25;         // Material Particulado
    float so2;          // Dioxido de Azufre
    float no2;          // Dioxido de Nitrogeno
    float co2;          // Dioxido de Carbono
    float temperatura;
    float velocidad_viento;
    float humedad;
} Medicion;

// Estructura principal del SECTOR (Zona de Quito)
typedef struct {
    int id;
    char nombre[LONGITUD_NOMBRE];
    
    // Base de datos historica (Memoria RAM)
    Medicion historial[DIAS_HISTORICOS]; 
    Medicion actual;                     
    
    // Resultados del analisis matematico
    float prediccion_futura;             
    float maximo_historico;              
    char nivel_contaminacion[20];        // Ej: BAJA, ALTA
    
    // Recomendaciones Multidimensionales
    char rec_social[250];
    char rec_cultural[250];
    char rec_ambiental[250];
    char rec_global[250];
    
    int alerta_activa; // 0=No, 1=Naranja, 2=Roja
} Sector;

// --- FUNCIONES DEL SISTEMA ---

// Utilidades
void limpiarBuffer();
void presionarEnter();
int leerEntero(const char* mensaje, int min, int max);
float leerFlotante(const char* mensaje, float min, float max);

// Logica Principal
void inicializarSectoresQuito(Sector sectores[]);
void ingresarMedicionActual(Sector* s);
void calcularPrediccionPonderada(Sector* s);
void evaluarAlertasDinamicas(Sector* s);
void generarRecomendaciones(Sector* s);
void verHistorialSector(Sector sectores[]);

// Archivos y Reportes
void guardarDatosIndividuales(Sector sectores[]);
void cargarDatosIndividuales(Sector sectores[]);
void generarReporteTexto(Sector sectores[]);

#endif