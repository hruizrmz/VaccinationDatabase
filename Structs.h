using namespace std;
struct Usuario {
    int userID = 0;
    wchar_t user[150] = L""; // unico
    wchar_t pwd[150] = L"";
    wchar_t apePat[150] = L"";
    wchar_t apeMat[150] = L"";
    wchar_t nombres[150] = L"";
} *firstUser, * auxUser, logUser;
struct Fecha {
    int dia = 0;
    int mes = 0;
    int anio = 0;
};
struct Persona {
    // usuario que la registro
    int userID = 0;
    // datos personales
    wchar_t apePat[150] = L"";
    wchar_t apeMat[150] = L"";
    wchar_t nombres[150] = L"";
    wchar_t curp[150] = L""; // unico
    wchar_t rfc[150] = L""; // unico
    Fecha fechaNac;
    // domicilio
    wchar_t calle[150] = L"";
    wchar_t colonia[150] = L"";
    wchar_t ciudad[150] = L"";
    wchar_t estado[150] = L"";
    wchar_t estCivil[150] = L"";
    wchar_t phone[150] = L"";
    // archivos
    wchar_t archivoID[300] = L"";
    wchar_t foto[300] = L"";
} *firstPer, * auxPer;
struct Vacuna {
    // usuario que la registro
    int userID = 0;
    //
    int dosis = 0;
    float precio = 0.0; // mayor a 0
    wchar_t tipo[150] = L"";
    wchar_t marca[150] = L"";
    wchar_t clave[150] = L""; // unica
    wchar_t desc[300] = L"";
} *firstVac, * auxVac;
struct Carne { // se genera uno por cada dosis de vacuna a una persona
    int userID = 0;
    int carnetID = 0; // generar un serial number
    //
    wchar_t curp[150] = L"";
    Persona* persona = NULL; // para apuntar a la persona
    wchar_t clave[150] = L"";
    Vacuna* vacuna = NULL; // y vacuna registradas
    int numDosis = 0; // en cual va segun la vacuna
    Fecha fechaDosis; // fecha de la dosis actual
    int diasEntreDosis = 0;
    Fecha sigDosis;
    wchar_t lote[150] = L""; // por ejemplo: PF01234
    wchar_t centroVacu[150] = L""; // centro de vacunacion
}*firstCarne, * auxCarne;