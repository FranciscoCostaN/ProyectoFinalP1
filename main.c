#include "funciones.h"

int main() {
    Sector sectores[MAX_SECTORES];
    int opcion;
    int sectorSeleccionado;

    // Intenta cargar datos guardados, si no hay, inicia simulacion
    cargarDatosIndividuales(sectores);

    do {
        printf("\n==================================================================================\n");
        printf("              SISTEMA DE MONITOREO MULTIGAS QUITO (REMMAQ)                        \n");
        printf("==================================================================================\n");
        printf("1. Ver Tablero de Control Completo (PM2.5, SO2, NO2, CO2)\n");
        printf("2. Actualizar Medicion de un Sector (Simular Sensor)\n");
        printf("3. Analisis Predictivo y Normativa (4 Dimensiones)\n");
        printf("4. Generar Reporte Oficial (.txt)\n");
        printf("5. Guardar Archivos .data y Salir\n");
        printf("6. Ver Historial Detallado por Sector\n");
        printf("==================================================================================\n");
        
        opcion = leerEntero("Seleccione opcion: ", 1, 6);

        switch (opcion) {
            case 1:
                printf("\n--- TABLERO DE CONTROL: GASES Y PARTICULAS ---\n");
                printf("%-16s | %-5s | %-5s | %-5s | %-6s | %-6s | %-8s\n", 
                       "SECTOR", "PM2.5", "SO2", "NO2", "CO2", "PRED", "ESTADO");
                printf("%-16s | %-5s | %-5s | %-5s | %-6s | %-6s | %-8s\n", 
                       "", "ug/m3", "ug/m3", "ug/m3", "ppm", "24h", "RIESGO");
                printf("----------------------------------------------------------------------------------\n");
                
                for(int i=0; i<MAX_SECTORES; i++) {
                    printf("%-16s | %-5.1f | %-5.1f | %-5.1f | %-6.0f | %-6.1f | %-8s\n", 
                           sectores[i].nombre, 
                           sectores[i].actual.pm25, sectores[i].actual.so2,
                           sectores[i].actual.no2, sectores[i].actual.co2,
                           sectores[i].prediccion_futura,
                           sectores[i].nivel_contaminacion);
                }
                printf("----------------------------------------------------------------------------------\n");
                // --- AQUI ESTA LA LINEA QUE FALTABA ---
                printf("Limites Ref (Max): PM2.5(%.0f), SO2(%.0f), NO2(%.0f), CO2(%.0f)\n", 
                       LIMITE_PM25, LIMITE_SO2, LIMITE_NO2, LIMITE_CO2);
                presionarEnter();
                break;

            case 2:
                printf("\n--- SELECCION DE ESTACION PARA ACTUALIZACION ---\n");
                for(int i=0; i<MAX_SECTORES; i++) {
                    printf("%d. %s\n", sectores[i].id, sectores[i].nombre);
                }
                sectorSeleccionado = leerEntero("Elija sector: ", 1, MAX_SECTORES);
                ingresarMedicionActual(&sectores[sectorSeleccionado - 1]);
                printf(">> Datos recibidos. Modelos matematicos recalibrados.\n");
                presionarEnter();
                break;

            case 3:
                printf("\n==================================================================================\n");
                printf("                     ANALISIS DE RIESGO Y NORMATIVA VIGENTE                       \n");
                printf("==================================================================================\n");
                printf("BASE TECNICA: Guias OMS (2021) y Norma TULSMA (Ecuador).\n");
                printf("LIMITES MAXIMOS (24h):\n");
                printf(" -> PM2.5: %.1f ug/m3\n", LIMITE_PM25);
                printf(" -> SO2:   %.1f ug/m3\n", LIMITE_SO2);
                printf(" -> NO2:   %.1f ug/m3\n", LIMITE_NO2);
                printf("----------------------------------------------------------------------------------\n");

                for(int i=0; i<MAX_SECTORES; i++) {
                    calcularPrediccionPonderada(&sectores[i]);
                    evaluarAlertasDinamicas(&sectores[i]);
                    generarRecomendaciones(&sectores[i]);
                    
                    printf("\n* SECTOR: %s *\n", sectores[i].nombre);
                    printf("  [Diagnostico]: RIESGO %s (Prediccion PM2.5: %.2f)\n", 
                           sectores[i].nivel_contaminacion, sectores[i].prediccion_futura);
                    
                    printf("  [Plan de Accion Multidimensional]:\n");
                    printf("   1. SOC: %s\n", sectores[i].rec_social);
                    printf("   2. CUL: %s\n", sectores[i].rec_cultural);
                    printf("   3. AMB: %s\n", sectores[i].rec_ambiental);
                    printf("   4. GLO: %s\n", sectores[i].rec_global);
                    printf("..................................................................................\n");
                }
                presionarEnter();
                break;

            case 4:
                generarReporteTexto(sectores);
                presionarEnter();
                break;

            case 5:
                printf("\nGuardando base de datos y saliendo...\n");
                guardarDatosIndividuales(sectores);
                break;
                
            case 6: 
                verHistorialSector(sectores);
                break;
        }

    } while (opcion != 5);

    return 0;
}