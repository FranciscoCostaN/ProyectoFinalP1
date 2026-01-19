#include "funciones.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main() {
    Sector sectores[MAX_SECTORES];
    int opcion;
    int sectorSeleccionado;

    cargarDatosIndividuales(sectores);

    do {
        printf("\n=================================================================\n");
        printf("      SISTEMA DE MONITOREO AMBIENTAL QUITO (REMMAQ) v3.1         \n");
        printf("=================================================================\n");
        printf("1. Ver Tablero de Control (5 Sectores Principales)\n");
        printf("2. Actualizar Medicion de un Sector\n");
        printf("3. Ver Prediccion y Alertas por Sector\n");
        printf("4. Generar Reporte Oficial (.txt con Fecha)\n");
        printf("5. Guardar Archivos .data y Salir\n");
        printf("=================================================================\n");
        
        opcion = leerEntero("Seleccione opcion: ", 1, 5);

        switch (opcion) {
            case 1:
                printf("\n--- TABLERO DE CONTROL: QUITO METROPOLITANO ---\n");
                printf("%-16s | %-5s | %-5s | %-6s | %-8s | %-8s\n", 
                       "SECTOR", "PM2.5", "TEMP", "VIENTO", "PRED(24h)", "ESTADO");
                printf("%-16s | %-5s | %-5s | %-6s | %-8s | %-8s\n", 
                       "", "ug/m3", "(C)", "km/h", "ug/m3", "RIESGO");
                printf("-----------------------------------------------------------------\n");
                
                for(int i=0; i<MAX_SECTORES; i++) {
                    printf("%-16s | %-5.1f | %-5.1f | %-6.1f | %-8.1f | %-8s\n", 
                           sectores[i].nombre, 
                           sectores[i].actual.pm25,
                           sectores[i].actual.temperatura,
                           sectores[i].actual.velocidad_viento,
                           sectores[i].prediccion_futura,
                           sectores[i].nivel_contaminacion);
                }
                printf("-----------------------------------------------------------------\n");
                presionarEnter();
                break;

            case 2:
                printf("\n--- SELECCION DE ESTACION DE MONITOREO ---\n");
                for(int i=0; i<MAX_SECTORES; i++) {
                    printf("%d. %s\n", sectores[i].id, sectores[i].nombre);
                }
                sectorSeleccionado = leerEntero("Elija sector a actualizar: ", 1, MAX_SECTORES);
                
                ingresarMedicionActual(&sectores[sectorSeleccionado - 1]);
                printf(">> Datos recibidos. Calibrando modelo del sector...\n");
                presionarEnter();
                break;

            case 3:
                printf("\n--- ANALISIS DETALLADO POR SECTOR ---\n");
                for(int i=0; i<MAX_SECTORES; i++) {
                    calcularPrediccionPonderada(&sectores[i]);
                    evaluarAlertasDinamicas(&sectores[i]);
                    generarRecomendaciones(&sectores[i]);
                    
                    printf("* %s *\n", sectores[i].nombre);
                    printf("  > Maximo Historico: %.2f\n", sectores[i].maximo_historico);
                    printf("  > Tendencia (Prediccion): %.2f\n", sectores[i].prediccion_futura);
                    printf("  > NIVEL DE RIESGO:        %s\n", sectores[i].nivel_contaminacion);
                    printf("  > ACCION RECOMENDADA:     %s\n", sectores[i].recomendacion);
                    printf("----------------------------------------------\n");
                }
                presionarEnter();
                break;

            case 4:
                generarReporteTexto(sectores);
                presionarEnter();
                break;

            case 5:
                printf("\nGuardando historial en archivos .data individuales...\n");
                guardarDatosIndividuales(sectores);
                printf("Sistema apagado.\n");
                break;
        }

    } while (opcion != 5);

    return 0;
}