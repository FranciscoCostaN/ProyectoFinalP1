#include "funciones.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int esDigito(char c) {
    if (c >= '0' && c <= '9') return 1;
    return 0;
}

void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void presionarEnter() {
    printf("\nPresione ENTER para continuar...");
    limpiarBuffer();
}

int leerEntero(const char* mensaje, int min, int max) {
    char buffer[100];
    int valido = 0;
    int resultado = 0;

    while (valido == 0) {
        printf("%s", mensaje);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            int len = 0;
            while (buffer[len] != '\0') {
                if (buffer[len] == '\n') {
                    buffer[len] = '\0';
                    break;
                }
                len++;
            }
            if (len == 0) continue; 

            int es_numero = 1;
            int inicio = 0;
            if (buffer[0] == '-') {
                inicio = 1;
                if (len == 1) es_numero = 0;
            }
            for (int i = inicio; i < len; i++) {
                if (esDigito(buffer[i]) == 0) {
                    es_numero = 0;
                    break;
                }
            }

            if (es_numero == 1) {
                int valor_temp = atoi(buffer);
                if (valor_temp >= min && valor_temp <= max) {
                    resultado = valor_temp;
                    valido = 1;
                } else {
                    printf("ERROR: Ingrese una opcion entre %d y %d.\n", min, max);
                }
            } else {
                printf("ERROR: Ingrese solo numeros enteros validos.\n");
            }
        }
    }
    return resultado;
}

float leerFlotante(const char* mensaje, float min, float max) {
    char buffer[100];
    int valido = 0;
    float resultado = 0.0;

    while (valido == 0) {
        printf("%s", mensaje);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            int len = 0;
            while (buffer[len] != '\0') {
                if (buffer[len] == '\n') {
                    buffer[len] = '\0';
                    break;
                }
                len++;
            }
            if (len == 0) continue;

            int es_valido = 1;
            int puntos = 0;
            int inicio = 0;
            if (buffer[0] == '-') {
                inicio = 1;
                if (len == 1) es_valido = 0;
            }

            for (int i = inicio; i < len; i++) {
                if (buffer[i] == '.') {
                    puntos++;
                    if (puntos > 1) es_valido = 0;
                } else if (esDigito(buffer[i]) == 0) {
                    es_valido = 0;
                }
            }

            if (es_valido == 1) {
                float valor_temp = atof(buffer);
                if (valor_temp >= min && valor_temp <= max) {
                    resultado = valor_temp;
                    valido = 1;
                } else {
                    printf("ERROR: El valor debe estar entre %.2f y %.2f.\n", min, max);
                }
            } else {
                printf("ERROR: Ingrese un numero decimal valido.\n");
            }
        }
    }
    return resultado;
}


void inicializarSectoresQuito(Sector sectores[]) {
    // 5 Sectores representativos de la red REMMAQ
    char n1[] = "Carapungo";      // Norte extremo (Seco, polvo)
    char n2[] = "Belisario";      // Centro Norte (Trafico alto)
    char n3[] = "Centro Historico"; // Centro (Calles angostas, smog)
    char n4[] = "Cotocollao";     // Norte
    char n5[] = "Tumbaco";        // Valle (Mas calido, generalmente limpio)

    strcpy(sectores[0].nombre, n1);
    strcpy(sectores[1].nombre, n2);
    strcpy(sectores[2].nombre, n3);
    strcpy(sectores[3].nombre, n4);
    strcpy(sectores[4].nombre, n5);
    
    printf("Cargando datos REMMAQ (Quito) en tiempo real...\n");

    // 1. CARAPUNGO (Norte seco)
    sectores[0].actual.pm25 = 28.0; // Moderado por polvo y canteras
    sectores[0].actual.temperatura = 12.0;
    sectores[0].actual.velocidad_viento = 18.0; // Viento fuerte tipico
    sectores[0].actual.humedad = 45.0;

    // 2. BELISARIO (Zona comercial/trafico)
    sectores[1].actual.pm25 = 35.0; // Trafico vehicular intenso
    sectores[1].actual.temperatura = 15.0;
    sectores[1].actual.velocidad_viento = 8.0;
    sectores[1].actual.humedad = 60.0;

    // 3. CENTRO HISTORICO (Congestion)
    sectores[2].actual.pm25 = 42.0; // Alto por buses y calles cerradas
    sectores[2].actual.temperatura = 16.0;
    sectores[2].actual.velocidad_viento = 5.0; // Poco flujo de aire
    sectores[2].actual.humedad = 55.0;

    // 4. COTOCOLLAO (Residencial norte)
    sectores[3].actual.pm25 = 12.0; // Aire relativamente bueno
    sectores[3].actual.temperatura = 13.0;
    sectores[3].actual.velocidad_viento = 10.0; 
    sectores[3].actual.humedad = 65.0;

    // 5. TUMBACO (Valle)
    sectores[4].actual.pm25 = 8.0;  // Aire limpio
    sectores[4].actual.temperatura = 21.0; // Mas calor
    sectores[4].actual.velocidad_viento = 6.0;
    sectores[4].actual.humedad = 40.0;

    for(int i = 0; i < MAX_SECTORES; i++) {
        sectores[i].id = i + 1;
        
        // Simular historial basado en el perfil del sector
        for(int j = 0; j < DIAS_HISTORICOS; j++) {
            sectores[i].historial[j].pm25 = sectores[i].actual.pm25 + ((rand() % 15) - 7);
            if(sectores[i].historial[j].pm25 < 0) sectores[i].historial[j].pm25 = 1.0;

            sectores[i].historial[j].temperatura = sectores[i].actual.temperatura + ((rand() % 4) - 2);
            sectores[i].historial[j].velocidad_viento = sectores[i].actual.velocidad_viento + ((rand() % 6) - 3);
            sectores[i].historial[j].humedad = sectores[i].actual.humedad + ((rand() % 10) - 5);
        }
        
        sectores[i].historial[0] = sectores[i].actual;

        // Ejecutar analisis inicial
        calcularPrediccionPonderada(&sectores[i]);
        evaluarAlertasDinamicas(&sectores[i]);
        generarRecomendaciones(&sectores[i]);
    }
    printf("Sistema REMMAQ listo. Sensores calibrados.\n");
}

void ingresarMedicionActual(Sector* s) {
    printf("\n--- Actualizar Sensor: %s ---\n", s->nombre);
    
    s->actual.pm25 = leerFlotante("PM2.5 Actual (0-500): ", 0, 500);
    s->actual.temperatura = leerFlotante("Temperatura (C): ", -10, 50);
    s->actual.velocidad_viento = leerFlotante("Viento (km/h): ", 0, 150);
    s->actual.humedad = leerFlotante("Humedad (%): ", 0, 100);
    
    for(int i = DIAS_HISTORICOS - 1; i > 0; i--) {
        s->historial[i] = s->historial[i-1];
    }
    s->historial[0] = s->actual; 
    
    calcularPrediccionPonderada(s);
    evaluarAlertasDinamicas(s);
    generarRecomendaciones(s);
}

void calcularPrediccionPonderada(Sector* s) {
    float suma_ponderada = 0;
    float total_pesos = 0;
    int dias = 10; 
    
    for(int i = 0; i < dias; i++) {
        float peso = (float)(dias - i); 
        suma_ponderada += s->historial[i].pm25 * peso;
        total_pesos += peso;
    }
    
    s->prediccion_futura = suma_ponderada / total_pesos;
    
    if(s->actual.velocidad_viento < 5.0) s->prediccion_futura *= 1.15; 
    if(s->actual.humedad > 80.0) s->prediccion_futura *= 0.80; 
}

void evaluarAlertasDinamicas(Sector* s) {
    float max = 0;
    for(int i=0; i<DIAS_HISTORICOS; i++) {
        if(s->historial[i].pm25 > max) max = s->historial[i].pm25;
    }
    s->maximo_historico = max;
    
    if (max < OMS_LIMITE_PM25) max = OMS_LIMITE_PM25;

    if (s->prediccion_futura < (max * 0.50)) {
        strcpy(s->nivel_contaminacion, "BAJA");
        s->alerta_activa = 0;
    } else if (s->prediccion_futura < (max * 0.75)) {
        strcpy(s->nivel_contaminacion, "MODERADA");
        s->alerta_activa = 0;
    } else if (s->prediccion_futura < (max * 0.90)) {
        strcpy(s->nivel_contaminacion, "ALTA");
        s->alerta_activa = 1; 
    } else {
        strcpy(s->nivel_contaminacion, "CRITICA");
        s->alerta_activa = 2; 
    }
}

void generarRecomendaciones(Sector* s) {
    if (strcmp(s->nivel_contaminacion, "BAJA") == 0) {
        strcpy(s->recomendacion, "CALIDAD BUENA: Aire limpio. Ideal para actividades al aire libre.");
    
    } else if (strcmp(s->nivel_contaminacion, "MODERADA") == 0) {
        strcpy(s->recomendacion, "PRECAUCION: Personas sensibles reducir esfuerzo fisico prolongado.");
    
    } else if (strcmp(s->nivel_contaminacion, "ALTA") == 0) {
        strcpy(s->recomendacion, "ALERTA NARANJA: Aire insalubre. Usar mascarilla. Evitar sector Centro/Trafico.");
    
    } else { 
        strcpy(s->recomendacion, "EMERGENCIA ROJA: PELIGRO. Restriccion vehicular total en el sector. No salir.");
    }
}


void guardarDatosIndividuales(Sector sectores[]) {
    char nombreArchivo[100];
    
    for(int i = 0; i < MAX_SECTORES; i++) {
        char nombreLimpio[50];
        strcpy(nombreLimpio, sectores[i].nombre);
        
        for(int j=0; j<strlen(nombreLimpio); j++){
            if(nombreLimpio[j] == ' ') nombreLimpio[j] = '_';
        }

        sprintf(nombreArchivo, "%s.data", nombreLimpio);
        
        FILE *archivo = fopen(nombreArchivo, "wb");
        if (archivo != NULL) {
            fwrite(&sectores[i], sizeof(Sector), 1, archivo);
            fclose(archivo);
            printf("-> Guardado: %s\n", nombreArchivo);
        } else {
            printf("Error al guardar %s\n", nombreArchivo);
        }
    }
    printf(">> Respaldo completo de sectores en archivos .data individualizados.\n");
}

void cargarDatosIndividuales(Sector sectores[]) {
    inicializarSectoresQuito(sectores); 
    
    char nombreArchivo[100];
    int encontrados = 0;

    printf("Buscando archivos historicos .data por sector...\n");

    for(int i = 0; i < MAX_SECTORES; i++) {
        char nombreLimpio[50];
        strcpy(nombreLimpio, sectores[i].nombre);
        for(int j=0; j<strlen(nombreLimpio); j++){
            if(nombreLimpio[j] == ' ') nombreLimpio[j] = '_';
        }

        sprintf(nombreArchivo, "%s.data", nombreLimpio);
        
        FILE *archivo = fopen(nombreArchivo, "rb");
        if (archivo != NULL) {
            fread(&sectores[i], sizeof(Sector), 1, archivo);
            fclose(archivo);
            encontrados++;
            
            calcularPrediccionPonderada(&sectores[i]);
            evaluarAlertasDinamicas(&sectores[i]);
            generarRecomendaciones(&sectores[i]);
        }
    }

    if(encontrados > 0) {
        printf(">> Se recuperaron datos historicos de %d sectores.\n", encontrados);
    } else {
        printf(">> No se encontraron archivos previos. Usando datos simulados.\n");
    }
}

void generarReporteTexto(Sector sectores[]) {
    FILE *archivo = fopen("reporte_remmaq_quito.txt", "w");
    if (archivo == NULL) {
        printf("Error al crear reporte TXT.\n");
        return;
    }

    // --- OBTENER FECHA Y HORA ACTUAL ---
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    fprintf(archivo, "==================================================\n");
    fprintf(archivo, "REPORTE DE CALIDAD DEL AIRE - DM QUITO\n");
    fprintf(archivo, "Sistema: SIGPCA v3.1 (Sectores)\n");
    fprintf(archivo, "Fecha de Generacion: %02d/%02d/%d %02d:%02d\n", 
            tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min);
    fprintf(archivo, "==================================================\n\n");

    for(int i=0; i<MAX_SECTORES; i++) {
        fprintf(archivo, "SECTOR: %s\n", sectores[i].nombre);
        fprintf(archivo, "  - Fecha Medicion:         %02d/%02d/%d\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
        fprintf(archivo, "  - Maximo Historico (30d): %.2f\n", sectores[i].maximo_historico);
        fprintf(archivo, "  - PM2.5 Actual:           %.2f ug/m3\n", sectores[i].actual.pm25);
        fprintf(archivo, "  - Viento:                 %.2f km/h\n", sectores[i].actual.velocidad_viento);
        fprintf(archivo, "  - Prediccion (24h):       %.2f\n", sectores[i].prediccion_futura);
        fprintf(archivo, "  - Nivel de Riesgo:        %s\n", sectores[i].nivel_contaminacion);
        fprintf(archivo, "  - ACCIONES:               %s\n\n", sectores[i].recomendacion);
        fprintf(archivo, "--------------------------------------------------\n");
    }
    fclose(archivo);
    printf(">> Reporte generado: 'reporte_remmaq_quito.txt' con fecha y hora.\n");
}