#include "funciones.h"

// --- VALIDACIONES DE ENTRADA ---
// Estas funciones evitan que el programa falle si el usuario escribe letras

int esDigito(char c) {
    return (c >= '0' && c <= '9');
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

    while (!valido) {
        printf("%s", mensaje);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            // Eliminar salto de linea
            buffer[strcspn(buffer, "\n")] = 0;
            if (strlen(buffer) == 0) continue;

            int es_numero = 1;
            for (int i = 0; i < strlen(buffer); i++) {
                if (!esDigito(buffer[i]) && !(i == 0 && buffer[i] == '-')) {
                    es_numero = 0; break;
                }
            }

            if (es_numero) {
                int valor = atoi(buffer);
                if (valor >= min && valor <= max) {
                    resultado = valor;
                    valido = 1;
                } else {
                    printf("Error: Ingrese un valor entre %d y %d.\n", min, max);
                }
            } else {
                printf("Error: Ingrese solo numeros enteros.\n");
            }
        }
    }
    return resultado;
}

float leerFlotante(const char* mensaje, float min, float max) {
    char buffer[100];
    int valido = 0;
    float resultado = 0.0;

    while (!valido) {
        printf("%s", mensaje);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            buffer[strcspn(buffer, "\n")] = 0;
            if (strlen(buffer) == 0) continue;

            int es_valido = 1;
            int puntos = 0;
            for (int i = 0; i < strlen(buffer); i++) {
                if (buffer[i] == '.') {
                    puntos++;
                    if (puntos > 1) es_valido = 0;
                } else if (!esDigito(buffer[i]) && !(i == 0 && buffer[i] == '-')) {
                    es_valido = 0;
                }
            }

            if (es_valido) {
                float valor = atof(buffer);
                if (valor >= min && valor <= max) {
                    resultado = valor;
                    valido = 1;
                } else {
                    printf("Error: El valor debe estar entre %.2f y %.2f.\n", min, max);
                }
            } else {
                printf("Error: Ingrese un numero decimal valido.\n");
            }
        }
    }
    return resultado;
}

// --- LOGICA DEL NEGOCIO (QUITO) ---

void inicializarSectoresQuito(Sector sectores[]) {
    // Definicion de sectores representativos
    strcpy(sectores[0].nombre, "Carapungo");      // Norte seco
    strcpy(sectores[1].nombre, "Belisario");      // Comercial/Trafico
    strcpy(sectores[2].nombre, "Centro Historico"); // Congestion
    strcpy(sectores[3].nombre, "Cotocollao");     // Residencial
    strcpy(sectores[4].nombre, "Tumbaco");        // Valle

    printf("Iniciando simulacion con perfiles base de REMMAQ...\n");

    // CARGA DE PERFILES BASE (Semilla inicial)
    // Sector 1: Carapungo (Polvo alto, viento fuerte)
    sectores[0].actual = (Medicion){28.0, 15.0, 20.0, 450.0, 12.0, 18.0, 45.0};

    // Sector 2: Belisario (Trafico alto -> NO2 y CO2 altos)
    sectores[1].actual = (Medicion){35.0, 10.0, 45.0, 600.0, 15.0, 8.0, 60.0};

    // Sector 3: Centro Historico (Congestion extrema)
    sectores[2].actual = (Medicion){42.0, 12.0, 50.0, 700.0, 16.0, 5.0, 55.0};

    // Sector 4: Cotocollao (Moderado)
    sectores[3].actual = (Medicion){12.0, 5.0, 10.0, 410.0, 13.0, 10.0, 65.0};

    // Sector 5: Tumbaco (Aire limpio)
    sectores[4].actual = (Medicion){8.0, 2.0, 5.0, 400.0, 21.0, 6.0, 40.0};

    // Generacion del historial inicial basado en la semilla
    for(int i = 0; i < MAX_SECTORES; i++) {
        sectores[i].id = i + 1;
        
        for(int j = 0; j < DIAS_HISTORICOS; j++) {
            // Simulamos variaciones diarias leves para llenar la base de datos
            sectores[i].historial[j] = sectores[i].actual;
            sectores[i].historial[j].pm25 += ((rand() % 10) - 5); // Variacion +/-
            if(sectores[i].historial[j].pm25 < 0) sectores[i].historial[j].pm25 = 1.0;
        }
        
        // Ejecutamos el modelo matematico inicial
        calcularPrediccionPonderada(&sectores[i]);
        evaluarAlertasDinamicas(&sectores[i]);
        generarRecomendaciones(&sectores[i]);
    }
    printf("Sistema inicializado correctamente.\n");
}

void ingresarMedicionActual(Sector* s) {
    printf("\n--- Actualizar Datos de Sensor: %s ---\n", s->nombre);
    
    // Ingreso validado de los 4 gases y clima
    s->actual.pm25 = leerFlotante("PM2.5 Actual (0-500): ", 0, 500);
    s->actual.so2  = leerFlotante("SO2 Actual (0-500): ", 0, 500);
    s->actual.no2  = leerFlotante("NO2 Actual (0-500): ", 0, 500);
    s->actual.co2  = leerFlotante("CO2 Actual (300-2000 ppm): ", 300, 2000);
    
    s->actual.temperatura = leerFlotante("Temperatura (C): ", -10, 50);
    s->actual.velocidad_viento = leerFlotante("Viento (km/h): ", 0, 150);
    s->actual.humedad = leerFlotante("Humedad (%): ", 0, 100);
    
    // Desplazamiento del historial (FIFO: First In First Out)
    for(int i = DIAS_HISTORICOS - 1; i > 0; i--) {
        s->historial[i] = s->historial[i-1];
    }
    s->historial[0] = s->actual; // El dato nuevo entra en la posicion 0 (HOY)
    
    // Recalculo inmediato de predicciones
    calcularPrediccionPonderada(s);
    evaluarAlertasDinamicas(s);
    generarRecomendaciones(s);
}

void calcularPrediccionPonderada(Sector* s) {
    float suma_ponderada = 0;
    float total_pesos = 0;
    int dias_analisis = 10; // Ventana de analisis reciente
    
    // Algoritmo de Promedio Ponderado Decreciente
    for(int i = 0; i < dias_analisis; i++) {
        float peso = (float)(dias_analisis - i); // Dia 0 tiene peso 10, dia 9 tiene peso 1
        suma_ponderada += s->historial[i].pm25 * peso;
        total_pesos += peso;
    }
    
    s->prediccion_futura = suma_ponderada / total_pesos;
    
    // Ajustes por factores climaticos (Correccion Ambiental)
    if(s->actual.velocidad_viento < 5.0) s->prediccion_futura *= 1.15; // Poco viento acumula gases
    if(s->actual.humedad > 80.0) s->prediccion_futura *= 0.80; // Lluvia limpia el aire
}

void evaluarAlertasDinamicas(Sector* s) {
    // 1. Buscamos el maximo registrado en el mes
    float max_historico = 0;
    for(int i=0; i<DIAS_HISTORICOS; i++) {
        if(s->historial[i].pm25 > max_historico) max_historico = s->historial[i].pm25;
    }
    s->maximo_historico = max_historico;
    if (max_historico < LIMITE_PM25) max_historico = LIMITE_PM25; // Piso minimo normativo

    // 2. Verificamos si algun gas especifico supera la norma OMS
    int gases_fuera_norma = 0;
    if(s->actual.so2 > LIMITE_SO2) gases_fuera_norma++;
    if(s->actual.no2 > LIMITE_NO2) gases_fuera_norma++;
    if(s->actual.co2 > LIMITE_CO2) gases_fuera_norma++;

    // 3. Determinamos nivel de riesgo combinado
    if (s->prediccion_futura < (max_historico * 0.50) && gases_fuera_norma == 0) {
        strcpy(s->nivel_contaminacion, "BAJA");
        s->alerta_activa = 0;
    } else if (s->prediccion_futura < (max_historico * 0.75) && gases_fuera_norma <= 1) {
        strcpy(s->nivel_contaminacion, "MODERADA");
        s->alerta_activa = 0;
    } else if (s->prediccion_futura < (max_historico * 0.90) || gases_fuera_norma == 2) {
        strcpy(s->nivel_contaminacion, "ALTA");
        s->alerta_activa = 1; // Alerta Naranja
    } else {
        strcpy(s->nivel_contaminacion, "CRITICA");
        s->alerta_activa = 2; // Alerta Roja
    }
}

void generarRecomendaciones(Sector* s) {
    // Asignacion de recomendaciones multidimensionales segun nivel de riesgo
    
    if (strcmp(s->nivel_contaminacion, "BAJA") == 0) {
        strcpy(s->rec_social,    "SOCIAL: Espacios seguros para ninos y tercera edad.");
        strcpy(s->rec_cultural,  "CULTURAL: Ideal para turismo patrimonial y caminatas.");
        strcpy(s->rec_ambiental, "AMBIENTAL: Mantener jardines y evitar quema de residuos.");
        strcpy(s->rec_global,    "GLOBAL: Cumplimiento ODS 11 (Ciudades Sostenibles).");
    
    } else if (strcmp(s->nivel_contaminacion, "MODERADA") == 0) {
        strcpy(s->rec_social,    "SOCIAL: Grupos sensibles reducir ejercicio intenso.");
        strcpy(s->rec_cultural,  "CULTURAL: Preferir eventos en interiores.");
        strcpy(s->rec_ambiental, "AMBIENTAL: Usar transporte compartido (Car-pooling).");
        strcpy(s->rec_global,    "GLOBAL: Nivel preventivo segun estandares OMS.");
    
    } else if (strcmp(s->nivel_contaminacion, "ALTA") == 0) {
        strcpy(s->rec_social,    "SOCIAL: Uso de mascarilla recomendado en paradas de bus.");
        strcpy(s->rec_cultural,  "CULTURAL: Evitar aglomeraciones al aire libre.");
        strcpy(s->rec_ambiental, "AMBIENTAL: Restriccion vehicular 'Pico y Placa' extendida.");
        strcpy(s->rec_global,    "GLOBAL: Riesgo de lluvia acida por SO2/NO2.");
    
    } else { // CRITICA
        strcpy(s->rec_social,    "SOCIAL: CONFINAMIENTO PREVENTIVO. Suspender clases.");
        strcpy(s->rec_cultural,  "CULTURAL: Cancelacion total de agenda publica.");
        strcpy(s->rec_ambiental, "AMBIENTAL: Cierre de industrias y restriccion total.");
        strcpy(s->rec_global,    "GLOBAL: PROTOCOLO DE EMERGENCIA CLIMATICA ACTIVADO.");
    }
}

void verHistorialSector(Sector sectores[]) {
    printf("\n--- CONSULTA DE REGISTROS HISTORICOS (30 DIAS) ---\n");
    for(int i=0; i<MAX_SECTORES; i++) {
        printf("%d. %s\n", sectores[i].id, sectores[i].nombre);
    }
    int opcion = leerEntero("Seleccione Sector: ", 1, MAX_SECTORES);
    Sector *s = &sectores[opcion - 1];

    printf("\n=========================================================================\n");
    printf("HISTORIAL SECTOR: %s\n", s->nombre);
    printf("=========================================================================\n");
    printf("%-5s | %-6s | %-6s | %-6s | %-6s | %-6s\n", "DIA", "PM2.5", "SO2", "NO2", "CO2", "TEMP");
    printf("-------------------------------------------------------------------------\n");
    
    for(int i=0; i<DIAS_HISTORICOS; i++) {
        char etiqueta[15];
        if(i==0) strcpy(etiqueta, "HOY");
        else sprintf(etiqueta, "-%d d", i);

        // Lectura directa de la memoria RAM donde esta el historial
        printf("%-5s | %-6.1f | %-6.1f | %-6.1f | %-6.0f | %-6.1f\n", 
               etiqueta, s->historial[i].pm25, s->historial[i].so2, 
               s->historial[i].no2, s->historial[i].co2, s->historial[i].temperatura);
    }
    printf("=========================================================================\n");
    presionarEnter();
}

// --- PERSISTENCIA (ARCHIVOS .DATA) ---

void guardarDatosIndividuales(Sector sectores[]) {
    char nombreArchivo[100];
    for(int i = 0; i < MAX_SECTORES; i++) {
        // Limpiamos nombre (Carapungo -> Carapungo.data)
        char nombreLimpio[50];
        strcpy(nombreLimpio, sectores[i].nombre);
        for(int j=0; j<strlen(nombreLimpio); j++){
            if(nombreLimpio[j] == ' ') nombreLimpio[j] = '_';
        }
        
        sprintf(nombreArchivo, "%s.data", nombreLimpio);
        
        // Escritura Binaria
        FILE *archivo = fopen(nombreArchivo, "wb");
        if (archivo != NULL) {
            fwrite(&sectores[i], sizeof(Sector), 1, archivo);
            fclose(archivo);
        }
    }
    printf(">> Copia de seguridad guardada en archivos .data individuales.\n");
}

void cargarDatosIndividuales(Sector sectores[]) {
    // 1. Cargamos simulacion por defecto por si no hay archivos
    inicializarSectoresQuito(sectores); 
    
    // 2. Intentamos buscar archivos guardados
    char nombreArchivo[100];
    int encontrados = 0;
    
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
            
            // Recalcular con los datos recuperados
            calcularPrediccionPonderada(&sectores[i]);
            evaluarAlertasDinamicas(&sectores[i]);
            generarRecomendaciones(&sectores[i]);
        }
    }
    
    if(encontrados > 0) printf(">> Se recuperaron %d sectores desde disco.\n", encontrados);
}

void generarReporteTexto(Sector sectores[]) {
    FILE *archivo = fopen("reporte_remmaq_multigas.txt", "w");
    if (archivo == NULL) {
        printf("Error al generar reporte.\n");
        return;
    }

    // Obtener fecha/hora actual
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    fprintf(archivo, "REPORTE INTEGRAL DE CALIDAD DEL AIRE - QUITO\n");
    fprintf(archivo, "Generado el: %02d/%02d/%d a las %02d:%02d\n\n", 
            tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min);
    
    fprintf(archivo, "NORMATIVA APLICADA (OMS/TULSMA):\n");
    fprintf(archivo, "PM2.5 Max: %.1f | SO2 Max: %.1f | NO2 Max: %.1f\n\n", 
            LIMITE_PM25, LIMITE_SO2, LIMITE_NO2);

    for(int i=0; i<MAX_SECTORES; i++) {
        fprintf(archivo, "SECTOR: %s\n", sectores[i].nombre);
        fprintf(archivo, "  Datos Actuales:\n");
        fprintf(archivo, "   - PM2.5: %.2f ug/m3\n", sectores[i].actual.pm25);
        fprintf(archivo, "   - SO2:   %.2f ug/m3\n", sectores[i].actual.so2);
        fprintf(archivo, "   - NO2:   %.2f ug/m3\n", sectores[i].actual.no2);
        fprintf(archivo, "   - CO2:   %.0f ppm\n", sectores[i].actual.co2);
        fprintf(archivo, "  Analisis:\n");
        fprintf(archivo, "   - Prediccion Tendencia: %.2f\n", sectores[i].prediccion_futura);
        fprintf(archivo, "   - NIVEL DE RIESGO: %s\n", sectores[i].nivel_contaminacion);
        fprintf(archivo, "  Recomendaciones:\n");
        fprintf(archivo, "   1. SOC: %s\n", sectores[i].rec_social);
        fprintf(archivo, "   2. CUL: %s\n", sectores[i].rec_cultural);
        fprintf(archivo, "   3. AMB: %s\n", sectores[i].rec_ambiental);
        fprintf(archivo, "   4. GLO: %s\n", sectores[i].rec_global);
        fprintf(archivo, "--------------------------------------------------\n");
    }
    fclose(archivo);
    printf(">> Reporte exportado exitosamente a 'reporte_remmaq_multigas.txt'.\n");
}