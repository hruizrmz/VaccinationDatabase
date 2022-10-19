// Helena Ruiz Ramírez 2076294
// Estructura de Datos Grupo 05
#include <Windows.h>
#include <CommCtrl.h>
#include <shellapi.h>
#include <math.h>
#include <atlstr.h>
#include <fstream>
#include <string>
#include <ctime> // para random
#include <cstdlib> // para random
#include <iomanip> // para setw output
#include "framework.h"
#include "Resource.h"
#include "LinkedList.h" // incluye Structs.h
///////////////////////////////
//////// DECLARACIONES ////////
#pragma region
INT_PTR CALLBACK LogIn(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK MainMenu(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK ExitCheck(HWND, UINT, WPARAM, LPARAM);

INT_PTR CALLBACK Info(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK RegPer(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK EditPer(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK DeletePer(HWND, UINT, WPARAM, LPARAM);

INT_PTR CALLBACK RegVac(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK EditVac(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK DeleteVac(HWND, UINT, WPARAM, LPARAM);

INT_PTR CALLBACK RegCarne(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK EditCarne(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK DeleteCarne(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK BuscarCarne(HWND, UINT, WPARAM, LPARAM);

INT_PTR CALLBACK RepNombre(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK RepCarne(HWND, UINT, WPARAM, LPARAM);
#pragma endregion
///////////////////////////////
///// VARIABLES GLOBALES //////
#pragma region
fstream file;
wofstream reportFile;
OPENFILENAME openFN;
bool mainOpen = false;
bool closeWnd = false;
int listCount, listSize;
int globalSel; // para saber la opcion seleccionada en un combo box de persona o vacuna
int personaSel, vacunaSel; // para obtener los datos del carne
wchar_t fechaBuffer[150];

LinkedList<Usuario> myUsers;
int globalID = 0; // guarda el numero total de usuarios
int logMessage = 1;
wchar_t userCheck[150]; // para validar el username
wchar_t pwdCheck[250]; // para validar el password
wchar_t apePat[150];
wchar_t apeMat[150];
wchar_t nombres[150];
wchar_t nombreComp[300];

LinkedList<Persona> myPeople;
wchar_t apePatP[150];
wchar_t apeMatP[150];
wchar_t nombresP[150];
wchar_t curp[150]; // unico
wchar_t oldCURP[150]; // para checar a la hora de editar
wchar_t rfc[150]; // unico
wchar_t oldRFC[150]; // para checar a la hora de editar
wchar_t diaBuffer[150];
wchar_t mesBuffer[150];
wchar_t anioBuffer[150];
int dia, mes, anio;
Fecha fechaNac;
wchar_t calle[150];
wchar_t colonia[150];
wchar_t ciudad[150];
wchar_t estado[150];
wchar_t estCivil[150];
wchar_t phone[150];
wchar_t archivoID[300];
wchar_t foto[300];
HBITMAP fotoBM;
bool hasFile = false;
bool fileEdit = false;
bool hasPhoto = false;
bool photoEdit = false;
bool foundPersona;
wchar_t nombreCompPersona[150];

LinkedList<Vacuna> myVaccines;
wchar_t tipo[150];
wchar_t marca[150];
wchar_t dosisBuffer[150];
int dosis;
wchar_t clave[150]; // unico
wchar_t oldClave[150]; // para checar a la hora de editar
wchar_t desc[300]; // unico
wchar_t precioBuffer[150];
float precio;
bool foundVac;

LinkedList<Carne> myCarnes;
wchar_t carnetIDBuffer[150];
int carnetID;
wchar_t numDosisBuffer[150];
int numDosis;
wchar_t dia2Buffer[150];
wchar_t mes2Buffer[150];
wchar_t anio2Buffer[150];
Fecha fechaDosis;
wchar_t diasDosisBuffer[150];
int diasEntreDosis;
wchar_t dia3Buffer[150];
wchar_t mes3Buffer[150];
wchar_t anio3Buffer[150];
Fecha sigDosis;
wchar_t lote[150];
wchar_t centroVacu[150];
wchar_t nombreCompCheck[150];
bool foundCarne;
bool chosePerson;
#pragma endregion
///////////////////////////////
////////// FUNCIONES //////////
#pragma region
// USUARIO //
#pragma region
void setNombreComp(Usuario usuario) { // concatena el nombre completo del usuario a la variable global nombreComp
    wcscpy_s(nombreComp, usuario.nombres);
    wcscat_s(nombreComp, L" ");
    wcscat_s(nombreComp, usuario.apePat);
    wcscat_s(nombreComp, L" ");
    wcscat_s(nombreComp, usuario.apeMat);
}
void getMaxIDUser() {
    file.open("userIDs.txt", ios::in);
    if (file.fail()) {
        MessageBox(NULL, L"No se pudo cargar el archivo 'userIDs.txt'", L"AVISO", MB_ICONERROR);
        return;
    }
    if (file.is_open()) {
        file >> globalID;
        file.close();
    }
}
void loadFromFileUser() {
    int ctr = 0;
    file.open("users.bin", ios::in | ios::binary);
    if (file.fail()) {
        MessageBox(NULL, L"No se pudo cargar el archivo 'users.bin'", L"AVISO", MB_ICONERROR);
        return;
    }
    if (file.is_open()) {
        while (!file.eof()) { // contamos el num de usuarios que ya existen
            Usuario* checkSize = new Usuario;
            file.read(reinterpret_cast<char*>(checkSize), sizeof(Usuario));
            delete reinterpret_cast<char*>(checkSize);
            ctr++;
        }
        file.close();
        if (ctr <= 1) { // no existen usuarios
            file.close();
            return;
        }
        file.open("users.bin", ios::in | ios::binary);
        for (int i = 1; i < ctr; i++) {
            if (myUsers.getHead() == NULL) { // si no se ha definido el primer usuario
                Usuario* temp = new Usuario;
                firstUser = new Usuario;
                file.read(reinterpret_cast<char*>(temp), sizeof(Usuario));
                firstUser->userID = temp->userID;
                wcscpy_s(firstUser->user, temp->user); // wcscpy_s: para asignar wchar_t
                wcscpy_s(firstUser->pwd, temp->pwd);
                wcscpy_s(firstUser->apePat, temp->apePat);
                wcscpy_s(firstUser->apeMat, temp->apeMat);
                wcscpy_s(firstUser->nombres, temp->nombres);
                myUsers.add(firstUser);
                delete reinterpret_cast<char*>(temp);
            }
            else {
                auxUser = myUsers.getTail()->getDato();
                Usuario* temp = new Usuario;
                auxUser = new Usuario;
                file.read(reinterpret_cast<char*>(temp), sizeof(Usuario));
                auxUser->userID = temp->userID;
                wcscpy_s(auxUser->user, temp->user); // wcscpy_s: para asignar wchar_t
                wcscpy_s(auxUser->pwd, temp->pwd);
                wcscpy_s(auxUser->apePat, temp->apePat);
                wcscpy_s(auxUser->apeMat, temp->apeMat);
                wcscpy_s(auxUser->nombres, temp->nombres);
                myUsers.add(auxUser);
                delete reinterpret_cast<char*>(temp);
            }
        }
        file.close();
    }
}
void saveMaxIDUser() {
    file.open("userIDs.txt", ios::out | ios::trunc);
    if (file.is_open()) {
        file << globalID;
        file.close();
    }
    else {
        MessageBox(NULL, L"No se pudo guardar el archivo 'userIDs.txt'", L"AVISO", MB_ICONERROR);
    }
}
bool findUser() {
    auxUser = myUsers.getHead()->getDato();
    listCount = 0;
    listSize = myUsers.getSize();
    while (wcscmp(auxUser->user, userCheck) != 0) {
        if (listCount == listSize) {
            logMessage = 1;
            return false;
        }
        else {
            listCount++;
            auxUser = myUsers.get(listCount);
        }
    }
    if (wcscmp(auxUser->pwd, pwdCheck) != 0) {
        logMessage = 2;
        return false;
    }
    logUser.userID = auxUser->userID;
    wcscpy_s(logUser.user, auxUser->user);
    wcscpy_s(logUser.pwd, auxUser->pwd);
    wcscpy_s(logUser.apePat, auxUser->apePat);
    wcscpy_s(logUser.apeMat, auxUser->apeMat);
    wcscpy_s(logUser.nombres, auxUser->nombres);
    setNombreComp(logUser);
    return true;
}
void saveUser() {
    if (firstUser == NULL) {
        firstUser = new Usuario;
        wcscpy_s(firstUser->user, userCheck);
        wcscpy_s(firstUser->pwd, pwdCheck);
        wcscpy_s(firstUser->apePat, apePat);
        wcscpy_s(firstUser->apeMat, apeMat);
        wcscpy_s(firstUser->nombres, nombres);
        firstUser->userID = globalID++;
        myUsers.add(firstUser);

        logUser.userID = firstUser->userID;
        wcscpy_s(logUser.user, firstUser->user);
        wcscpy_s(logUser.pwd, firstUser->pwd);
        wcscpy_s(logUser.apePat, firstUser->apePat);
        wcscpy_s(logUser.apeMat, firstUser->apeMat);
        wcscpy_s(logUser.nombres, firstUser->nombres);
        setNombreComp(logUser);
    }
    else {
        auxUser = new Usuario;
        wcscpy_s(auxUser->user, userCheck);
        wcscpy_s(auxUser->pwd, pwdCheck);
        wcscpy_s(auxUser->apePat, apePat);
        wcscpy_s(auxUser->apeMat, apeMat);
        wcscpy_s(auxUser->nombres, nombres);
        auxUser->userID = globalID++;
        myUsers.add(auxUser);

        logUser.userID = auxUser->userID;
        wcscpy_s(logUser.user, auxUser->user);
        wcscpy_s(logUser.pwd, auxUser->pwd);
        wcscpy_s(logUser.apePat, auxUser->apePat);
        wcscpy_s(logUser.apeMat, auxUser->apeMat);
        wcscpy_s(logUser.nombres, auxUser->nombres);
        setNombreComp(logUser);
    }
}
bool editUser() {
    auxUser = myUsers.get(logUser.userID);
    if (wcscmp(auxUser->pwd, pwdCheck) != 0) {
        MessageBox(NULL, L"La contraseña es incorrecta", L"AVISO", MB_ICONERROR);
        return false;
    }
    wcscpy_s(auxUser->apePat, apePat);
    wcscpy_s(auxUser->apeMat, apeMat);
    wcscpy_s(auxUser->nombres, nombres);
    wcscpy_s(logUser.apePat, auxUser->apePat);
    wcscpy_s(logUser.apeMat, auxUser->apeMat);
    wcscpy_s(logUser.nombres, auxUser->nombres);
    setNombreComp(logUser);
    return true;
}
#pragma endregion
// PERSONAS //
#pragma region
void loadFromFilePeople() {
    int ctr = 0;
    file.open("people.bin", ios::in | ios::binary);
    if (file.fail()) {
        MessageBox(NULL, L"No se pudo cargar el archivo 'people.bin'", L"AVISO", MB_ICONERROR);
        return;
    }
    if (file.is_open()) {
        while (!file.eof()) {
            Persona* checkSize = new Persona;
            file.read(reinterpret_cast<char*>(checkSize), sizeof(Persona));
            delete reinterpret_cast<char*>(checkSize);
            ctr++;
        }
        file.close();
        if (ctr <= 1) {
            file.close();
            return;
        }
        file.open("people.bin", ios::in | ios::binary);
        for (int i = 1; i < ctr; i++) {
            if (myPeople.getHead() == NULL) {
                Persona* temp = new Persona;
                firstPer = new Persona;
                file.read(reinterpret_cast<char*>(temp), sizeof(Persona));
                firstPer->userID = temp->userID;
                wcscpy_s(firstPer->apePat, temp->apePat);
                wcscpy_s(firstPer->apeMat, temp->apeMat);
                wcscpy_s(firstPer->nombres, temp->nombres);
                wcscpy_s(firstPer->curp, temp->curp);
                wcscpy_s(firstPer->rfc, temp->rfc);
                firstPer->fechaNac = temp->fechaNac;
                wcscpy_s(firstPer->calle, temp->calle);
                wcscpy_s(firstPer->colonia, temp->colonia);
                wcscpy_s(firstPer->ciudad, temp->ciudad);
                wcscpy_s(firstPer->estado, temp->estado);
                wcscpy_s(firstPer->estCivil, temp->estCivil);
                wcscpy_s(firstPer->phone, temp->phone);
                wcscpy_s(firstPer->archivoID, temp->archivoID);
                wcscpy_s(firstPer->foto, temp->foto);
                myPeople.add(firstPer);
                delete reinterpret_cast<char*>(temp);
            }
            else {
                auxPer = myPeople.getTail()->getDato();
                Persona* temp = new Persona;
                auxPer = new Persona;
                file.read(reinterpret_cast<char*>(temp), sizeof(Persona));
                auxPer->userID = temp->userID;
                wcscpy_s(auxPer->apePat, temp->apePat);
                wcscpy_s(auxPer->apeMat, temp->apeMat);
                wcscpy_s(auxPer->nombres, temp->nombres);
                wcscpy_s(auxPer->curp, temp->curp);
                wcscpy_s(auxPer->rfc, temp->rfc);
                auxPer->fechaNac = temp->fechaNac;
                wcscpy_s(auxPer->calle, temp->calle);
                wcscpy_s(auxPer->colonia, temp->colonia);
                wcscpy_s(auxPer->ciudad, temp->ciudad);
                wcscpy_s(auxPer->estado, temp->estado);
                wcscpy_s(auxPer->estCivil, temp->estCivil);
                wcscpy_s(auxPer->phone, temp->phone);
                wcscpy_s(auxPer->archivoID, temp->archivoID);
                wcscpy_s(auxPer->foto, temp->foto);
                myPeople.add(auxPer);
                delete reinterpret_cast<char*>(temp);
            }
        }
        file.close();
    }
}
void savePersona() {
    if (firstPer == NULL) {
        firstPer = new Persona;
        wcscpy_s(firstPer->apePat, apePatP);
        wcscpy_s(firstPer->apeMat, apeMatP);
        wcscpy_s(firstPer->nombres, nombresP);
        wcscpy_s(firstPer->curp, curp);
        wcscpy_s(firstPer->rfc, rfc);
        firstPer->fechaNac = fechaNac;
        wcscpy_s(firstPer->calle, calle);
        wcscpy_s(firstPer->colonia, colonia);
        wcscpy_s(firstPer->ciudad, ciudad);
        wcscpy_s(firstPer->estado, estado);
        wcscpy_s(firstPer->estCivil, estCivil);
        wcscpy_s(firstPer->phone, phone);
        wcscpy_s(firstPer->archivoID, archivoID);
        wcscpy_s(firstPer->foto, foto);
        firstPer->userID = logUser.userID;
        myPeople.add(firstPer);
    }
    else {
        auxPer = new Persona;
        wcscpy_s(auxPer->apePat, apePatP);
        wcscpy_s(auxPer->apeMat, apeMatP);
        wcscpy_s(auxPer->nombres, nombresP);
        wcscpy_s(auxPer->curp, curp);
        wcscpy_s(auxPer->rfc, rfc);
        auxPer->fechaNac = fechaNac;
        wcscpy_s(auxPer->calle, calle);
        wcscpy_s(auxPer->colonia, colonia);
        wcscpy_s(auxPer->ciudad, ciudad);
        wcscpy_s(auxPer->estado, estado);
        wcscpy_s(auxPer->estCivil, estCivil);
        wcscpy_s(auxPer->phone, phone);
        wcscpy_s(auxPer->archivoID, archivoID);
        wcscpy_s(auxPer->foto, foto);
        auxPer->userID = logUser.userID;
        myPeople.add(auxPer);
    }
}
void editPersona(Persona*& auxPer) {
    wcscpy_s(auxPer->apePat, apePatP);
    wcscpy_s(auxPer->apeMat, apeMatP);
    wcscpy_s(auxPer->nombres, nombresP);
    wcscpy_s(auxPer->curp, curp);
    wcscpy_s(auxPer->rfc, rfc);
    auxPer->fechaNac = fechaNac;
    wcscpy_s(auxPer->calle, calle);
    wcscpy_s(auxPer->colonia, colonia);
    wcscpy_s(auxPer->ciudad, ciudad);
    wcscpy_s(auxPer->estado, estado);
    wcscpy_s(auxPer->estCivil, estCivil);
    wcscpy_s(auxPer->phone, phone);
    if (fileEdit) {
        wcscpy_s(auxPer->archivoID, archivoID);
    }
    if (photoEdit) {
        wcscpy_s(auxPer->foto, foto);
    }
}
Persona* findPersona(wchar_t curp[150]) {
    listSize = myPeople.getSize();
    listCount = 0;
    foundPersona = false;
    while ((listCount < listSize) && !foundPersona) {
        auxPer = myPeople.get(listCount);
        if (auxPer->userID == logUser.userID) {
            if (wcscmp(auxPer->curp, curp) == 0) {
                foundPersona = true;
            }
        }
        listCount++;
    }
    return auxPer;
}
void setNombreCompPersona(Persona persona) { // concatena el nombre completo del usuario a la variable global nombreComp
    wcscpy_s(nombreCompPersona, persona.nombres);
    wcscat_s(nombreCompPersona, L" ");
    wcscat_s(nombreCompPersona, persona.apePat);
    wcscat_s(nombreCompPersona, L" ");
    wcscat_s(nombreCompPersona, persona.apeMat);
}
#pragma endregion
// VACUNAS //
#pragma region
void loadFromFileVaccines() {
    int ctr = 0;
    file.open("vaccines.bin", ios::in | ios::binary);
    if (file.fail()) {
        MessageBox(NULL, L"No se pudo cargar el archivo 'vaccines.bin'", L"AVISO", MB_ICONERROR);
        return;
    }
    if (file.is_open()) {
        while (!file.eof()) {
            Vacuna* checkSize = new Vacuna;
            file.read(reinterpret_cast<char*>(checkSize), sizeof(Vacuna));
            delete reinterpret_cast<char*>(checkSize);
            ctr++;
        }
        file.close();
        if (ctr <= 1) {
            file.close();
            return;
        }
        file.open("vaccines.bin", ios::in | ios::binary);
        for (int i = 1; i < ctr; i++) {
            if (myVaccines.getHead() == NULL) {
                Vacuna* temp = new Vacuna;
                firstVac = new Vacuna;
                file.read(reinterpret_cast<char*>(temp), sizeof(Vacuna));
                firstVac->userID = temp->userID;
                firstVac->dosis = temp->dosis;
                firstVac->precio = temp->precio;
                wcscpy_s(firstVac->tipo, temp->tipo);
                wcscpy_s(firstVac->marca, temp->marca);
                wcscpy_s(firstVac->clave, temp->clave);
                wcscpy_s(firstVac->desc, temp->desc);
                myVaccines.add(firstVac);
                delete reinterpret_cast<char*>(temp);
            }
            else {
                auxVac = myVaccines.getTail()->getDato();
                Vacuna* temp = new Vacuna;
                auxVac = new Vacuna;
                file.read(reinterpret_cast<char*>(temp), sizeof(Vacuna));
                auxVac->userID = temp->userID;
                auxVac->dosis = temp->dosis;
                auxVac->precio = temp->precio;
                wcscpy_s(auxVac->tipo, temp->tipo);
                wcscpy_s(auxVac->marca, temp->marca);
                wcscpy_s(auxVac->clave, temp->clave);
                wcscpy_s(auxVac->desc, temp->desc);
                myVaccines.add(auxVac);
                delete reinterpret_cast<char*>(temp);
            }
        }
        file.close();
    }
}
void saveVacuna() {
    if (firstVac == NULL) {
        firstVac = new Vacuna;
        firstVac->userID = logUser.userID;
        firstVac->dosis = dosis;
        firstVac->precio = precio;
        wcscpy_s(firstVac->tipo, tipo);
        wcscpy_s(firstVac->marca, marca);
        wcscpy_s(firstVac->clave, clave);
        wcscpy_s(firstVac->desc, desc);
        myVaccines.add(firstVac);
    }
    else {
        auxVac = new Vacuna;
        auxVac->userID = logUser.userID;
        auxVac->dosis = dosis;
        auxVac->precio = precio;
        wcscpy_s(auxVac->tipo, tipo);
        wcscpy_s(auxVac->marca, marca);
        wcscpy_s(auxVac->clave, clave);
        wcscpy_s(auxVac->desc, desc);
        myVaccines.add(auxVac);
    }
}
void editVacuna(Vacuna*& auxVac) {
    auxVac->dosis = dosis;
    auxVac->precio = precio;
    wcscpy_s(auxVac->tipo, tipo);
    wcscpy_s(auxVac->marca, marca);
    wcscpy_s(auxVac->clave, clave);
    wcscpy_s(auxVac->desc, desc);
}
Vacuna* findVacuna(wchar_t clave[150]) {
    listSize = myVaccines.getSize();
    listCount = 0;
    foundPersona = false;
    while ((listCount < listSize) && !foundPersona) {
        auxVac = myVaccines.get(listCount);
        if (auxVac->userID == logUser.userID) {
            if (wcscmp(auxVac->clave, clave) == 0) {
                foundPersona = true;
            }
        }
        listCount++;
    }
    return auxVac;
}
#pragma endregion
// CARNE //
#pragma region
void loadfromFileCarne() {
    int ctr = 0;
    file.open("carnes.bin", ios::in | ios::binary);
    if (file.fail()) {
        MessageBox(NULL, L"No se pudo cargar el archivo 'carnes.bin'", L"AVISO", MB_ICONERROR);
        return;
    }
    if (file.is_open()) {
        while (!file.eof()) {
            Carne* checkSize = new Carne;
            file.read(reinterpret_cast<char*>(checkSize), sizeof(Carne));
            delete reinterpret_cast<char*>(checkSize);
            ctr++;
        }
        file.close();
        if (ctr <= 1) {
            file.close();
            return;
        }
        file.open("carnes.bin", ios::in | ios::binary);
        for (int i = 1; i < ctr; i++) {
            if (myCarnes.getHead() == NULL) {
                Carne* temp = new Carne;
                firstCarne = new Carne;
                file.read(reinterpret_cast<char*>(temp), sizeof(Carne));
                firstCarne->userID = temp->userID;
                firstCarne->carnetID = temp->carnetID;
                //
                wcscpy_s(firstCarne->curp, temp->curp);
                firstCarne->persona = findPersona(temp->curp);
                wcscpy_s(firstCarne->clave, temp->clave);
                firstCarne->vacuna = findVacuna(temp->clave);
                //
                firstCarne->numDosis = temp->numDosis;
                firstCarne->fechaDosis = temp->fechaDosis;
                firstCarne->diasEntreDosis = temp->diasEntreDosis;
                firstCarne->sigDosis = temp->sigDosis;
                wcscpy_s(firstCarne->lote, temp->lote);
                wcscpy_s(firstCarne->centroVacu, temp->centroVacu);                
                myCarnes.assortedInsert(firstCarne);
                delete reinterpret_cast<char*>(temp);
            }
            else {
                auxCarne = myCarnes.getTail()->getDato();
                Carne* temp = new Carne;
                auxCarne = new Carne;
                file.read(reinterpret_cast<char*>(temp), sizeof(Carne));
                auxCarne->userID = temp->userID;
                auxCarne->carnetID = temp->carnetID;
                //
                wcscpy_s(auxCarne->curp, temp->curp);
                auxCarne->persona = findPersona(temp->curp);
                wcscpy_s(auxCarne->clave, temp->clave);
                auxCarne->vacuna = findVacuna(temp->clave);
                //
                auxCarne->numDosis = temp->numDosis;
                auxCarne->fechaDosis = temp->fechaDosis;
                auxCarne->diasEntreDosis = temp->diasEntreDosis;
                auxCarne->sigDosis = temp->sigDosis;
                wcscpy_s(auxCarne->lote, temp->lote);
                wcscpy_s(auxCarne->centroVacu, temp->centroVacu);
                myCarnes.assortedInsert(auxCarne);
                delete reinterpret_cast<char*>(temp);
            }
        }
        file.close();
    }
}
void saveCarne() {
    if (firstCarne == NULL) {
        firstCarne = new Carne;
        firstCarne->carnetID = carnetID;
        wcscpy_s(firstCarne->curp, curp);
        firstCarne->persona = findPersona(curp);
        wcscpy_s(firstCarne->clave, clave);
        firstCarne->vacuna = findVacuna(clave);
        firstCarne->numDosis = numDosis;
        firstCarne->fechaDosis = fechaDosis;
        firstCarne->diasEntreDosis = diasEntreDosis;
        firstCarne->sigDosis = sigDosis;
        firstCarne->sigDosis = sigDosis;
        wcscpy_s(firstCarne->lote, lote);
        wcscpy_s(firstCarne->centroVacu, centroVacu);
        firstCarne->userID = logUser.userID;
        myCarnes.assortedInsert(firstCarne);
    }
    else {
        auxCarne = new Carne;
        auxCarne->carnetID = carnetID;
        wcscpy_s(auxCarne->curp, curp);
        auxCarne->persona = findPersona(curp);
        wcscpy_s(auxCarne->clave, clave);
        auxCarne->vacuna = findVacuna(clave);
        auxCarne->numDosis = numDosis;
        auxCarne->fechaDosis = fechaDosis;
        auxCarne->diasEntreDosis = diasEntreDosis;
        auxCarne->sigDosis = sigDosis;
        wcscpy_s(auxCarne->curp, curp);
        wcscpy_s(auxCarne->clave, clave);
        wcscpy_s(auxCarne->lote, lote);
        wcscpy_s(auxCarne->centroVacu, centroVacu);
        auxCarne->userID = logUser.userID;
        myCarnes.assortedInsert(auxCarne);
    }
}
void editCarne(Carne*& auxCarne) {
    auxCarne->numDosis = numDosis;
    auxCarne->fechaDosis = fechaDosis;
    auxCarne->sigDosis = sigDosis;
    auxCarne->diasEntreDosis = diasEntreDosis;
    wcscpy_s(auxCarne->curp, curp);
    wcscpy_s(auxCarne->clave, clave);
    wcscpy_s(auxCarne->lote, lote);
    wcscpy_s(auxCarne->centroVacu, centroVacu);
}
#pragma endregion
// GENERALES //
#pragma region
template <class T>
void saveToFile(LinkedList<T> list, string filename) {
    if (list.getHead() != NULL) {
        T* aux;
        listCount = 0;
        listSize = list.getSize();
        file.open(filename, ios::binary | ios::out);
        if (file.is_open()) {
            while (listCount < listSize) {
                aux = list.get(listCount);
                file.write(reinterpret_cast<char*>(aux), sizeof(T));
                listCount++;
            }
            file.close();
        }
        else {
            MessageBox(NULL, L"No se pudo guardar el archivo bin", L"Warning", MB_ICONERROR);
        }
    }
}
void sumFecha(Fecha fechaOriginal, int diasNuevos, Fecha &fechaNueva) {
    int dia = fechaOriginal.dia + diasNuevos;
    int mes = fechaOriginal.mes;
    int anio = fechaOriginal.anio;
    bool dias = false;
    while (!dias) { // va recorriendo los valores de la fecha hasta que ya no haya valores que sumar/ajustar
        if (mes == 1 || mes == 3 || mes == 5 || mes == 7 || mes == 8 || mes == 10 || mes == 12) {
            if (dia > 31) { // si los dias son mayores que los del mes actual
                dia -= 31; // se mueven los dias a 1 de nuevo pero se agrega un mes
                mes++;
            }
        }
        else if (mes == 4 || mes == 6 || mes == 9 || mes == 11) {
            if (dia > 30) {
                dia -= 30;
                mes++;
            }
        }
        else if (mes == 2) {
            if (anio % 4 == 0) {
                if (dia > 29) { // lo mismo pero se ajusta a febrero y los años bisiestos
                    dia -= 29;
                    mes++;
                }
            }
            else {
                if (dia > 28) {
                    dia -= 28;
                    mes++;
                }
            }
        }
        while (mes > 12) { // si el mes es mayor a 12, significa que se tiene que cambiar el año
            mes -= 12;
            anio++;
        }
        if (mes == 1 || mes == 3 || mes == 5 || mes == 7 || mes == 8 || mes == 10 || mes == 12) {
            if (dia <= 31) // si los dias ya no sobrepasan el total del mes, podemos terminar de sumar dias
                dias = true;
        }
        else if (mes == 4 || mes == 6 || mes == 9 || mes == 11) {
            if (dia < 30)
                dias = true;
        }
        else if (mes == 2) {
            if (anio % 4 == 0) {
                if (dia < 29)
                    dias = true;
            }
            else {
                if (dia < 28)
                    dias = true;
            }
        }
    }
    // guardamos los valores finales en la fecha indicada
    fechaNueva.dia = dia;
    fechaNueva.mes = mes;
    fechaNueva.anio = anio;
}
bool isVaccinated(Fecha fechaDosis) {
    SYSTEMTIME hoy;
    GetLocalTime(&hoy);
    if (fechaDosis.anio <= hoy.wYear) {
        if (fechaDosis.anio == hoy.wYear) {
            if ((fechaDosis.mes == hoy.wMonth && fechaDosis.dia < hoy.wDay) || fechaDosis.mes < hoy.wMonth)
                return true; // si es un mes anterior, ya se vacuno. si es el mismo mes, y el dia es anterior, ya se vacuno
            else
                return false; // si el mes es posterior, no se ha vacunado
        }
        return true; // si tiene un año anterior, ya se vacuno
    }
    else {
        return false; // si tiene un año posterior, no se ha vacunado
    }
}
void setFechaCompleta(Fecha f) {
    swprintf_s(diaBuffer, 30, L"%d", f.dia);
    wcscpy_s(fechaBuffer, diaBuffer);
    wcscat_s(fechaBuffer, L"/");
    swprintf_s(mesBuffer, 30, L"%d", f.mes);
    wcscat_s(fechaBuffer, mesBuffer);
    wcscat_s(fechaBuffer, L"/");
    swprintf_s(anioBuffer, 30, L"%d", f.anio);
    wcscat_s(fechaBuffer, anioBuffer);
}
#pragma endregion
// SORTS//
#pragma region
// en mi caso, mis # carnes se acomodan automaticamente de manera ascendiente al ser añadidos a la lista ligada.
// esto con la finalidad de que el método de búsqueda binaria funcione adecuadamente.
// sin embargo, al crear el reporte ordenado por nombre, este orden se altera.
// por eso, el método heap sort se manda a llamar cada vez que se genera un reporte o se consultan las ventanas de los carnés.
void heapify(LinkedList<Carne> carnes, int n, int i) {
    int mayor = i; // como quiero un MaxHeap, la raíz debe ser el mayor # del heap
    int izq = 2 * i + 1; // posicion del nodo izquierdo
    int der = 2 * i + 2;  // posicion del nodo derecho

    // mientras que el nodo sea hijo de la raíz y el ID sea mayor que el de la raíz...
    if ((izq < n) && (carnes.get(izq)->carnetID > carnes.get(mayor)->carnetID)) {
        mayor = izq;
    }
    // si no, se checa lo mismo pero con el nodo derecho
    if ((der < n) && (carnes.get(der)->carnetID > carnes.get(mayor)->carnetID)) {
        mayor = der;
    }
    // si el # más grande se encontró en uno de los nodos...
    if (mayor != i) {
        carnes.swap(carnes.get(i), carnes.get(mayor), i, mayor); // se intercambia el mayor con la raíz
        heapify(carnes, n, mayor); // continua recursivamente hasta que ninguno de los nodos sea mayor que la raíz
    }
}
void heapSort(LinkedList<Carne> carnes, int n) {
    // antes de acomodar el arreglo, tiene que asegurarse de que tenga la estructura MaxHeap...
    for (int i = n / 2 - 1; i >= 0; i--) { // reacomoda el arreglo una rama a la vez
        heapify(carnes, n, i); // n es el tamaño de la rama, i es la raiz de la rama
    }
    // una vez que se tiene el MaxHeap, acomoda y separa un elemento a la vez con cada sort del arbol binario
    for (int i = n - 1; i >= 0; i--) {
        carnes.swap(carnes.get(0), carnes.get(i), 0, i); // con cada ciclo, intercambia la raiz actual al final del arreglo (el mayor se va al final)
        heapify(carnes, i, 0); // el tamaño 'i' de cada rama va disminuyendo conforme llegamos a la raíz principal
    }                          // vuelve a acomodar el MaxHeap, y sigue cambiando la raiz hasta que el arreglo queda ordenado
}
////
int particion(LinkedList<Carne> list, int inicio, int fin) {
    // Agarra como pivote el ultimo dato en la lista/particion
    wchar_t* pivote = list.get(fin)->persona->apePat;
    wchar_t* comp;
    wchar_t* auxPivote1 = list.get(fin)->persona->apeMat;
    wchar_t* auxPivote2 = list.get(fin)->persona->nombres;

    // Se inicializa el pointer para el dato mayor
    int i = (inicio - 1);

    // Compara cada dato en la lista con el pivote...
    for (int j = inicio; j < fin; j++) {
        comp = list.get(j)->persona->apePat;
        // wcscmp compara dos wchar_t* y regresa -1 cuando el string1 es menor al string2
        int check = wcscmp(comp, pivote);
        if (wcscmp(comp, pivote) < 0) {
            // Si el dato en 'j' es menor al pivote, se cambia con la pos de 'i'
            i++;
            list.swap(list.get(i), list.get(j), i, j);
        }
        else if (wcscmp(comp, pivote) == 0) { // si tienen el mismo apellido paterno, checa el materno
            comp = list.get(j)->persona->apeMat;
            if (wcscmp(comp, auxPivote1) < 0) {
                i++;
                list.swap(list.get(i), list.get(j), i, j);
            }
            else if (wcscmp(comp, auxPivote1) == 0) { // lo mismo, pero ahora con sus nombres
                comp = list.get(j)->persona->nombres;
                if (wcscmp(comp, auxPivote2) < 0) {
                    i++;
                    list.swap(list.get(i), list.get(j), i, j);
                } // si el nombre es idéntico, no hay cambios
            }
        }
    }

    // Cambia el pivote con el ultimo valor mayor (+ 1) que se encontro en el recorrido
    // el pivote ya se había definido como el fin de la particion
    auxPivote1 = list.get(i+1)->persona->curp;
    auxPivote2 = list.get(fin)->persona->curp;
    list.swap(list.get(i+1), list.get(fin), i+1, fin);

    // Regresa el proximo punto de particion
    return (i + 1);
}
void quickSort(LinkedList<Carne> list, int inicio, int fin) {
    // La primera vez es.... inicio = 0 .... fin = tamaño de la lista
    if (inicio < fin) {
        // Separa la lista según los elementos menores y mayores a el pivote 'inicio'
        int p = particion(list, inicio, fin);

        // Arregla la partición izquierda (menor a mi pivote)
        quickSort(list, inicio, p - 1);

        // Arregla la partición derecha (mayor a mi pivote)
        quickSort(list, p + 1, fin);
    }
    // Cuando los elementos de la partición se hayan terminado de acomodar, termina la recursividad
}
////
void saveReport(LinkedList<Carne> list, string filename) {
    if (list.getHead() != NULL) {
        Carne* aux;
        listCount = 0;
        listSize = list.getSize();
        reportFile.open(filename, ios::trunc | ios::out);
        if (reportFile.is_open()) {
            reportFile << setw(9) << "CARNET ID" << setw(16) << "FECHA DE DOSIS" << setw(18) << "APELLIDO PATERNO" << setw(18) << "APELLIDO MATERNO" << setw(18) << "NOMBRE(S)";
            reportFile << setw(18) << "FECHA NACIMIENTO" << setw(15) << "CALLE" << setw(18) << "COLONIA" << setw(15) << "CIUDAD";
            reportFile << setw(10) << "ESTADO" << setw(14) << "TELÉFONO" << setw(45) << "DOCUMENTO DE IDENTIFICACIÓN" << endl;
            while (listCount < listSize) {
                aux = list.get(listCount);
                if (isVaccinated(aux->fechaDosis)) {
                    setFechaCompleta(aux->fechaDosis);
                    reportFile << setw(9) << aux->carnetID << setw(16) << fechaBuffer << setw(18) << aux->persona->apePat << setw(18) << aux->persona->apeMat << setw(18) << aux->persona->nombres;
                    setFechaCompleta(aux->persona->fechaNac);
                    reportFile << setw(18) << fechaBuffer << setw(15) << aux->persona->calle << setw(18) << aux->persona->colonia << setw(15) << aux->persona->ciudad;
                    reportFile << setw(10) << aux->persona->estado << setw(14) << aux->persona->phone << setw(62) << aux->persona->archivoID << endl;
                }
                listCount++;
            }
            reportFile.close();
        }
        else {
            MessageBox(NULL, L"No se pudo guardar el reporte .txt", L"Warning", MB_ICONERROR);
        }
    }
}
#pragma endregion
#pragma endregion
///////////////////////////////
INT_PTR CALLBACK LogIn(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) { // La primera ventana
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
    case WM_INITDIALOG: {
        return (INT_PTR)TRUE;
    }
    case WM_COMMAND: {
        if (LOWORD(wParam) == IDL_INGRESAR) {
            SendMessage(GetDlgItem(hDlg, IDL_USER), WM_GETTEXT, sizeof(userCheck) / sizeof(userCheck[0]), (LPARAM)userCheck);
            SendMessage(GetDlgItem(hDlg, IDL_PWD), WM_GETTEXT, sizeof(pwdCheck) / sizeof(pwdCheck[0]), (LPARAM)pwdCheck);
            if (wcslen(userCheck) == 0 || wcslen(pwdCheck) == 0) { // checa si el usuario lleno ambos campos
                MessageBox(NULL, L"Por favor llene todos los campos", L"AVISO", MB_ICONERROR);
                break;
            }
            if (firstUser != NULL) {
                if (findUser()) { // si logra encontrar al usuario en la base de datos
                    MessageBox(NULL, L"¡Log-in exitoso!", L"AVISO", MB_ICONINFORMATION);
                    loadFromFilePeople();
                    loadFromFileVaccines();
                    loadfromFileCarne();
                    EndDialog(hDlg, LOWORD(wParam));
                    setNombreComp(logUser);
                    DialogBox(NULL, MAKEINTRESOURCE(IDD_MAIN), hDlg, MainMenu);
                }
                else { // si no lo logra hacer
                    switch (logMessage) {
                    case 1: {
                        MessageBox(NULL, L"No existe ese usuario", L"AVISO", MB_ICONERROR);
                        break;
                    }
                    case 2: {
                        MessageBox(NULL, L"La contraseña es incorrecta", L"AVISO", MB_ICONERROR);
                        break;
                    }
                    default:
                        break;
                    }
                }
            }
            else {
                MessageBox(NULL, L"Todavia no se ha registrado ningun usuario", L"AVISO", MB_ICONERROR);
                break;
            }
        }
        else if (LOWORD(wParam) == IDL_REGISTRO) {
            SendMessage(GetDlgItem(hDlg, IDL_USER), WM_GETTEXT, sizeof(userCheck) / sizeof(userCheck[0]), (LPARAM)userCheck);
            SendMessage(GetDlgItem(hDlg, IDL_PWD), WM_GETTEXT, sizeof(pwdCheck) / sizeof(pwdCheck[0]), (LPARAM)pwdCheck);
            if (wcslen(userCheck) == 0 || wcslen(pwdCheck) == 0) { // checa si el usuario lleno ambos campos
                MessageBox(NULL, L"Por favor llene todos los campos", L"AVISO", MB_ICONERROR);
                break;
            }
            if (firstUser != NULL) {
                listSize = myUsers.getSize();
                listCount = 0;
                while (listCount < listSize) {
                    auxUser = myUsers.get(listCount);
                    if (wcscmp(auxUser->user, userCheck) == 0) { // si ya existe el usuario, cancela
                        MessageBox(NULL, L"Ese usuario ya existe, escriba uno diferente", L"AVISO", MB_ICONERROR);
                        return (INT_PTR)FALSE;
                    }
                    listCount++;
                }
            }
            loadFromFilePeople();
            loadFromFileVaccines();
            loadfromFileCarne();
            EndDialog(hDlg, LOWORD(wParam));
            DialogBox(NULL, MAKEINTRESOURCE(IDD_INFO), hDlg, Info);
        }
        break;
    }
    case WM_CLOSE: {
        DialogBox(NULL, MAKEINTRESOURCE(IDD_EXIT), hDlg, ExitCheck);
        break;
    }
    default:
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK MainMenu(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) { // Para manejar el menu principal
    UNREFERENCED_PARAMETER(lParam);
    mainOpen = true;

    switch (message) {
    case WM_INITDIALOG: {
        SetDlgItemText(hDlg, ID_SHOWUSER, nombreComp);
        return (INT_PTR)TRUE;
    }
    case WM_COMMAND: {
        //SetDlgItemText(hDlg, ID_SHOWUSER, nombreComp);
        if (LOWORD(wParam) == IDU_INFO) {
            DialogBox(NULL, MAKEINTRESOURCE(IDD_INFO), hDlg, Info);
        }
        if (LOWORD(wParam) == IDP_REGISTRO) {
            DialogBox(NULL, MAKEINTRESOURCE(IDD_REGPER), hDlg, RegPer);
        }
        if (LOWORD(wParam) == IDP_EDITAR) {
            DialogBox(NULL, MAKEINTRESOURCE(IDD_EDITPER), hDlg, EditPer);
        }
        if (LOWORD(wParam) == IDP_DELETE) {
            DialogBox(NULL, MAKEINTRESOURCE(IDD_DELPER), hDlg, DeletePer);
        }
        if (LOWORD(wParam) == IDV_REGISTRO) {
            DialogBox(NULL, MAKEINTRESOURCE(IDD_REGVAC), hDlg, RegVac);
        }
        if (LOWORD(wParam) == IDV_EDITAR) {
            DialogBox(NULL, MAKEINTRESOURCE(IDD_EDITVAC), hDlg, EditVac);
        }
        if (LOWORD(wParam) == IDV_DELETE) {
            DialogBox(NULL, MAKEINTRESOURCE(IDD_DELVAC), hDlg, DeleteVac);
        }
        if (LOWORD(wParam) == IDC_REGISTRO) {
            DialogBox(NULL, MAKEINTRESOURCE(IDD_REGCARNE), hDlg, RegCarne);
        }
        if (LOWORD(wParam) == IDC_EDITAR) {
            DialogBox(NULL, MAKEINTRESOURCE(IDD_EDITCARNE), hDlg, EditCarne);
        }
        if (LOWORD(wParam) == IDC_DELETE) {
            DialogBox(NULL, MAKEINTRESOURCE(IDD_DELCARNE), hDlg, DeleteCarne);
        }
        if (LOWORD(wParam) == IDC_FIND) {
            DialogBox(NULL, MAKEINTRESOURCE(IDD_BUSQCARNE), hDlg, BuscarCarne);
        }
        if (LOWORD(wParam) == IDR_N) {
            DialogBox(NULL, MAKEINTRESOURCE(IDD_REPNOMBRE), hDlg, RepNombre);
        }
        if (LOWORD(wParam) == IDR_C) {
            DialogBox(NULL, MAKEINTRESOURCE(IDD_REPCARNE), hDlg, RepCarne);
        }
        if (LOWORD(wParam) == ID_SALIR) {
            DialogBox(NULL, MAKEINTRESOURCE(IDD_EXIT), hDlg, ExitCheck);
        }
        int n = myCarnes.getSize();
        heapSort(myCarnes, n); // vuelve a ordenar los carnes por # de manera ascendiente (en caso de haber usado el quicksort).
                                  // esto es necesario para que la insercion de datos siempre se haga de manera ordenada
                                    // y se pueda realizar la busqueda binaria.
        break;
    }
    default:
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK ExitCheck(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) { // Determina si se cierra o no el programa/la ventana
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
    case WM_INITDIALOG: {
        return (INT_PTR)TRUE;
    }
    case WM_COMMAND: {
        EndDialog(hDlg, LOWORD(wParam));
        if (LOWORD(wParam) == EXIT_NO) closeWnd = false;
        if (LOWORD(wParam) == EXIT_CLOSE) closeWnd = true;
        if (LOWORD(wParam) == EXIT_YES) {
            saveMaxIDUser();
            saveToFile(myUsers, "users.bin");
            saveToFile(myPeople, "people.bin");
            saveToFile(myVaccines, "vaccines.bin");
            saveToFile(myCarnes, "carnes.bin");
            myUsers.deleteList();
            myPeople.deleteList();
            myVaccines.deleteList();
            myCarnes.deleteList();
            PostQuitMessage(0);
        }
        break;
    }
    default:
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK Info(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) { // Para la info del vendedor (primera ventana)
    UNREFERENCED_PARAMETER(lParam);

    switch (message) {
    case WM_INITDIALOG: {
        if (logUser.user != L"") {
            //SetDlgItemText(hDlg, IDU_SHOWUSER, logUser.user);
            SetDlgItemText(hDlg, IDU_SHOWUSER, userCheck);
            SetDlgItemText(hDlg, IDU_APEPAT, logUser.apePat);
            SetDlgItemText(hDlg, IDU_APEMAT, logUser.apeMat);
            SetDlgItemText(hDlg, IDU_NOMBRES, logUser.nombres);
        } else {
            SetDlgItemText(hDlg, IDU_SHOWUSER, userCheck);
        }
        return (INT_PTR)TRUE;
    }
    case WM_COMMAND: {
        if (LOWORD(wParam) == IDU_SAVE) {
            SendMessage(GetDlgItem(hDlg, IDU_APEPAT), WM_GETTEXT, sizeof(apePat) / sizeof(apePat[0]), (LPARAM)apePat);
            SendMessage(GetDlgItem(hDlg, IDU_APEMAT), WM_GETTEXT, sizeof(apeMat) / sizeof(apeMat[0]), (LPARAM)apeMat);
            SendMessage(GetDlgItem(hDlg, IDU_NOMBRES), WM_GETTEXT, sizeof(nombres) / sizeof(nombres[0]), (LPARAM)nombres);
            if (wcslen(apePat) == 0 || wcslen(apeMat) == 0 || wcslen(nombres) == 0) { // checa si el usuario lleno ambos campos
                MessageBox(NULL, L"Por favor complete toda la información", L"AVISO", MB_ICONERROR);
                break;
            }
            for (int i = 0; i < wcslen(apePat); i++) {
                if (!iswalpha(apePat[i]) && (apePat[i] != ' ')) {
                    MessageBox(NULL, L"El apellido paterno solo puede tener caracteres alfabéticos", L"AVISO", MB_ICONERROR);
                    return (INT_PTR)FALSE;
                }
            }
            for (int i = 0; i < wcslen(apeMat); i++) {
                if (!iswalpha(apeMat[i]) && (apeMat[i] != ' ')) {
                    MessageBox(NULL, L"El apellido materno solo puede tener caracteres alfabéticos", L"AVISO", MB_ICONERROR);
                    return (INT_PTR)FALSE;
                }
            }
            for (int i = 0; i < wcslen(nombres); i++) {
                if (!iswalpha(nombres[i]) && (nombres[i] != ' ')) {
                    MessageBox(NULL, L"El nombre solo puede tener caracteres alfabéticos", L"AVISO", MB_ICONERROR);
                    return (INT_PTR)FALSE;
                }
            }
            if (wcslen(logUser.user) == 0) {
                saveUser();
            }
            else {
                editUser();
            }
            MessageBox(NULL, L"¡Registro de usuario exitoso!", L"AVISO", MB_ICONINFORMATION);
            EndDialog(hDlg, LOWORD(wParam));
            if (!mainOpen) DialogBox(NULL, MAKEINTRESOURCE(IDD_MAIN), hDlg, MainMenu);
        }
        break;
    }
    case WM_CLOSE: {
        DialogBox(NULL, MAKEINTRESOURCE(IDD_CLOSE), hDlg, ExitCheck);
        if (closeWnd) {
            EndDialog(hDlg, LOWORD(wParam));
            if (!mainOpen) PostQuitMessage(0);
        }
        break;
    }
    default:
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK RegPer(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);

    switch (message) {
    case WM_INITDIALOG: {
        hasPhoto = false;
        wmemset(foto, L'\0', 200);
        SetDlgItemText(hDlg, IDP_SHOWUSER, nombreComp);
        hasFile = false;
        return (INT_PTR)TRUE;
    }
    case WM_COMMAND: {
        if (LOWORD(wParam) == IDP_CARGAR) { // carga un archivo de identificacion
            ZeroMemory(&openFN, sizeof(openFN));
            openFN.lStructSize = sizeof(openFN);
            openFN.hwndOwner = hDlg;
            openFN.lpstrFile = archivoID;
            openFN.lpstrFilter = L"Portable Document Format\0*.pdf";
            openFN.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
            openFN.lpstrFile[0] = NULL;
            openFN.nMaxFile = sizeof(archivoID);
            openFN.nFilterIndex = 0;
            if (GetOpenFileName(&openFN)) {
                SetDlgItemText(hDlg, IDP_ARCHIVO, archivoID);
                hasFile = true;
            }
        }
        if (LOWORD(wParam) == IDP_CARGARF) { // carga una foto de perfil
            ZeroMemory(&openFN, sizeof(openFN));
            openFN.lStructSize = sizeof(openFN);
            openFN.hwndOwner = hDlg;
            openFN.lpstrFile = foto;
            openFN.lpstrFilter = L"Mapa de bits\0*.bmp";
            openFN.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
            openFN.lpstrFile[0] = NULL;
            openFN.nMaxFile = sizeof(foto);
            openFN.nFilterIndex = 0;
            if (GetOpenFileName(&openFN)) {
                fotoBM = (HBITMAP)LoadImage(NULL, foto, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
                SendDlgItemMessage(hDlg, IDP_FOTO, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)fotoBM);
                hasPhoto = true;
            }
        }
        if (LOWORD(wParam) == IDP_REG) {
            SendMessage(GetDlgItem(hDlg, IDP_APEPAT), WM_GETTEXT, sizeof(apePatP) / sizeof(apePatP[0]), (LPARAM)apePatP);
            SendMessage(GetDlgItem(hDlg, IDP_APEMAT), WM_GETTEXT, sizeof(apeMatP) / sizeof(apeMatP[0]), (LPARAM)apeMatP);
            SendMessage(GetDlgItem(hDlg, IDP_NOMBRES), WM_GETTEXT, sizeof(nombresP) / sizeof(nombresP[0]), (LPARAM)nombresP);
            SendMessage(GetDlgItem(hDlg, IDP_CURP), WM_GETTEXT, sizeof(curp) / sizeof(curp[0]), (LPARAM)curp);
            SendMessage(GetDlgItem(hDlg, IDP_RFC), WM_GETTEXT, sizeof(rfc) / sizeof(rfc[0]), (LPARAM)rfc);
            SendMessage(GetDlgItem(hDlg, IDP_DIA), WM_GETTEXT, sizeof(diaBuffer) / sizeof(diaBuffer[0]), (LPARAM)diaBuffer);
            dia = _wtoi(diaBuffer);
            fechaNac.dia = dia;
            SendMessage(GetDlgItem(hDlg, IDP_MES), WM_GETTEXT, sizeof(mesBuffer) / sizeof(mesBuffer[0]), (LPARAM)mesBuffer);
            mes = _wtoi(mesBuffer);
            fechaNac.mes = mes;
            SendMessage(GetDlgItem(hDlg, IDP_ANIO), WM_GETTEXT, sizeof(anioBuffer) / sizeof(anioBuffer[0]), (LPARAM)anioBuffer);
            anio = _wtoi(anioBuffer);
            fechaNac.anio = anio;
            SendMessage(GetDlgItem(hDlg, IDP_CIVIL), WM_GETTEXT, sizeof(estCivil) / sizeof(estCivil[0]), (LPARAM)estCivil);

            SendMessage(GetDlgItem(hDlg, IDP_CALLE), WM_GETTEXT, sizeof(calle) / sizeof(calle[0]), (LPARAM)calle);
            SendMessage(GetDlgItem(hDlg, IDP_COLONIA), WM_GETTEXT, sizeof(colonia) / sizeof(colonia[0]), (LPARAM)colonia);
            SendMessage(GetDlgItem(hDlg, IDP_CIUDAD), WM_GETTEXT, sizeof(ciudad) / sizeof(ciudad[0]), (LPARAM)ciudad);
            SendMessage(GetDlgItem(hDlg, IDP_ESTADO), WM_GETTEXT, sizeof(estado) / sizeof(estado[0]), (LPARAM)estado);
            SendMessage(GetDlgItem(hDlg, IDP_TEL), WM_GETTEXT, sizeof(phone) / sizeof(phone[0]), (LPARAM)phone);

            if (wcslen(apePatP) == 0 || wcslen(apeMatP) == 0 || wcslen(nombresP) == 0 || wcslen(curp) == 0 || wcslen(rfc) == 0 || wcslen(diaBuffer) == 0 || wcslen(mesBuffer) == 0 || wcslen(anioBuffer) == 0 || wcslen(estCivil) == 0
                || wcslen(calle) == 0 || wcslen(colonia) == 0 || wcslen(ciudad) == 0 || wcslen(estado) == 0 || wcslen(phone) == 0 || !hasFile || !hasPhoto) {
                MessageBox(NULL, L"Por favor complete toda la información", L"AVISO", MB_ICONERROR);
                break;
            }
            for (int i = 0; i < wcslen(apePatP); i++) {
                if (!iswalpha(apePatP[i]) && (apePatP[i] != ' ')) {
                    MessageBox(NULL, L"El apellido paterno solo puede tener caracteres alfabéticos", L"AVISO", MB_ICONERROR);
                    return (INT_PTR)FALSE;
                }
            }
            for (int i = 0; i < wcslen(apeMatP); i++) {
                if (!iswalpha(apeMatP[i]) && (apeMatP[i] != ' ')) {
                    MessageBox(NULL, L"El apellido materno solo puede tener caracteres alfabéticos", L"AVISO", MB_ICONERROR);
                    return (INT_PTR)FALSE;
                }
            }
            for (int i = 0; i < wcslen(nombresP); i++) {
                if (!iswalpha(nombresP[i]) && (nombresP[i] != ' ')) {
                    MessageBox(NULL, L"El nombre solo puede tener caracteres alfabéticos", L"AVISO", MB_ICONERROR);
                    return (INT_PTR)FALSE;
                }
            }
            if (firstPer != NULL) {
                listCount = 0;
                listSize = myPeople.getSize();
                while (listCount < listSize) {
                    auxPer = myPeople.get(listCount);
                    // if (logUser.userID == auxPer->userID) { } // en caso de que las personas son unicas solo por usuario
                    if (wcscmp(auxPer->curp, curp) == 0) {
                        MessageBox(NULL, L"Ese CURP ya fue registrado.", L"AVISO", MB_ICONERROR);
                        return (INT_PTR)FALSE;
                    }
                    if (wcscmp(auxPer->rfc, rfc) == 0) {
                        MessageBox(NULL, L"Ese RFC ya fue registrado.", L"AVISO", MB_ICONERROR);
                        return (INT_PTR)FALSE;
                    }
                    listCount++;
                }
            }
            savePersona();
            MessageBox(NULL, L"¡El registro de la persona fue exitoso!", L"AVISO", MB_ICONINFORMATION);
            EndDialog(hDlg, LOWORD(wParam));
            if (!mainOpen) DialogBox(NULL, MAKEINTRESOURCE(IDD_MAIN), hDlg, MainMenu);
        }
        break;
    }
    case WM_CLOSE: {
        DialogBox(NULL, MAKEINTRESOURCE(IDD_CLOSE), hDlg, ExitCheck);
        if (closeWnd) {
            EndDialog(hDlg, LOWORD(wParam));
            if (!mainOpen) PostQuitMessage(0);
        }
        break;
    }
    default:
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK EditPer(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) { // Para editar cualquier persona con un CURP valido
    UNREFERENCED_PARAMETER(lParam);

    switch (message) {
    case WM_INITDIALOG: {
        SetDlgItemText(hDlg, IDP_SHOWUSER, nombreComp);
        if (firstPer != NULL) {
            listSize = myPeople.getSize();
            listCount = 0;
            while (listCount < listSize) { // muestra las claves de las vacunas registradas por el usuario actual
                auxPer = myPeople.get(listCount);
                if (auxPer->userID == logUser.userID) SendDlgItemMessage(hDlg, IDP_COMBO, CB_ADDSTRING, NULL, (LPARAM)auxPer->curp);
                listCount++;
            }
        }
        if (SendDlgItemMessage(hDlg, IDP_COMBO, CB_GETCOUNT, NULL, NULL) == 0) {
            MessageBox(NULL, L"No hay ninguna persona registrada que editar", L"AVISO", MB_ICONERROR);
            EndDialog(hDlg, LOWORD(wParam));
            if (!mainOpen) DialogBox(NULL, MAKEINTRESOURCE(IDD_MAIN), hDlg, MainMenu);
        }
        else {
            fileEdit = false; // para reconocer si la dir del archivo se cambio o no
            photoEdit = false;
            fotoBM = NULL;
        }
        return (INT_PTR)TRUE;
    }
    case WM_COMMAND: {
        if (HIWORD(wParam) == CBN_SELENDOK) { // cuando el usuario seleccione una opcion del combo box
            int sel = SendDlgItemMessage(hDlg, IDP_COMBO, CB_GETCURSEL, NULL, NULL);
            SendDlgItemMessage(hDlg, IDP_COMBO, CB_GETLBTEXT, sel, (LPARAM)curp);
            listSize = myPeople.getSize();
            listCount = 0;
            foundPersona = false;
            while ((listCount < listSize) && !foundPersona) {
                auxPer = myPeople.get(listCount);
                if (auxPer->userID == logUser.userID) {
                    SendMessage(GetDlgItem(hDlg, IDP_COMBO), CB_GETLBTEXT, sizeof(curp) / sizeof(curp[0]), (LPARAM)curp);
                    if (wcscmp(auxPer->curp, curp) == 0) {
                        SetDlgItemText(hDlg, IDP_APEPAT, auxPer->apePat);
                        SetDlgItemText(hDlg, IDP_APEMAT, auxPer->apeMat);
                        SetDlgItemText(hDlg, IDP_NOMBRES, auxPer->nombres);
                        SetDlgItemText(hDlg, IDP_CURP, auxPer->curp);
                        SendMessage(GetDlgItem(hDlg, IDP_CURP), WM_GETTEXT, sizeof(oldCURP) / sizeof(oldCURP[0]), (LPARAM)oldCURP);
                        SetDlgItemText(hDlg, IDP_RFC, auxPer->rfc);
                        SendMessage(GetDlgItem(hDlg, IDP_RFC), WM_GETTEXT, sizeof(oldRFC) / sizeof(oldRFC[0]), (LPARAM)oldRFC);
                        swprintf_s(diaBuffer, 30, L"%d", auxPer->fechaNac.dia);
                        SetDlgItemText(hDlg, IDP_DIA, diaBuffer);
                        swprintf_s(mesBuffer, 30, L"%d", auxPer->fechaNac.mes);
                        SetDlgItemText(hDlg, IDP_MES, mesBuffer);
                        swprintf_s(anioBuffer, 30, L"%d", auxPer->fechaNac.anio);
                        SetDlgItemText(hDlg, IDP_ANIO, anioBuffer);
                        SetDlgItemText(hDlg, IDP_CIVIL, auxPer->estCivil);
                        SetDlgItemText(hDlg, IDP_CALLE, auxPer->calle);
                        SetDlgItemText(hDlg, IDP_COLONIA, auxPer->colonia);
                        SetDlgItemText(hDlg, IDP_CIUDAD, auxPer->ciudad);
                        SetDlgItemText(hDlg, IDP_ESTADO, auxPer->estado);
                        SetDlgItemText(hDlg, IDP_COLONIA, auxPer->colonia);
                        SetDlgItemText(hDlg, IDP_TEL, auxPer->phone);
                        SetDlgItemText(hDlg, IDP_ARCHIVO, auxPer->archivoID);
                        fotoBM = (HBITMAP)LoadImage(NULL, auxPer->foto, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
                        SendDlgItemMessage(hDlg, IDP_FOTO, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)fotoBM);
                        globalSel = listCount;
                        foundPersona = true;
                    }
                }
                listCount++;
            }
        }
        if (LOWORD(wParam) == IDP_CARGAR) { // carga una foto de perfil
            ZeroMemory(&openFN, sizeof(openFN));
            openFN.lStructSize = sizeof(openFN);
            openFN.hwndOwner = hDlg;
            openFN.lpstrFile = archivoID;
            openFN.lpstrFilter = L"Portable Document Format\0*.pdf";
            openFN.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
            openFN.lpstrFile[0] = NULL;
            openFN.nMaxFile = sizeof(archivoID);
            openFN.nFilterIndex = 0;
            if (GetOpenFileName(&openFN)) {
                fileEdit = true;
                SetDlgItemText(hDlg, IDP_ARCHIVO, archivoID);
            }
        }
        if (LOWORD(wParam) == IDP_CARGARF) { // carga una foto de perfil
            ZeroMemory(&openFN, sizeof(openFN));
            openFN.lStructSize = sizeof(openFN);
            openFN.hwndOwner = hDlg;
            openFN.lpstrFile = foto;
            openFN.lpstrFilter = L"Mapa de bits\0*.bmp";
            openFN.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
            openFN.lpstrFile[0] = NULL;
            openFN.nMaxFile = sizeof(foto);
            openFN.nFilterIndex = 0;
            if (GetOpenFileName(&openFN)) {
                fotoBM = (HBITMAP)LoadImage(NULL, foto, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
                SendDlgItemMessage(hDlg, IDP_FOTO, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)fotoBM);
                photoEdit = true;
            }
        }
        if (LOWORD(wParam) == IDP_SAVE) {
            SendMessage(GetDlgItem(hDlg, IDP_APEPAT), WM_GETTEXT, sizeof(apePatP) / sizeof(apePatP[0]), (LPARAM)apePatP);
            SendMessage(GetDlgItem(hDlg, IDP_APEMAT), WM_GETTEXT, sizeof(apeMatP) / sizeof(apeMatP[0]), (LPARAM)apeMatP);
            SendMessage(GetDlgItem(hDlg, IDP_NOMBRES), WM_GETTEXT, sizeof(nombresP) / sizeof(nombresP[0]), (LPARAM)nombresP);
            SendMessage(GetDlgItem(hDlg, IDP_CURP), WM_GETTEXT, sizeof(curp) / sizeof(curp[0]), (LPARAM)curp);
            SendMessage(GetDlgItem(hDlg, IDP_RFC), WM_GETTEXT, sizeof(rfc) / sizeof(rfc[0]), (LPARAM)rfc);
            SendMessage(GetDlgItem(hDlg, IDP_DIA), WM_GETTEXT, sizeof(diaBuffer) / sizeof(diaBuffer[0]), (LPARAM)diaBuffer);
            dia = _wtoi(diaBuffer);
            fechaNac.dia = dia;
            SendMessage(GetDlgItem(hDlg, IDP_MES), WM_GETTEXT, sizeof(mesBuffer) / sizeof(mesBuffer[0]), (LPARAM)mesBuffer);
            mes = _wtoi(mesBuffer);
            fechaNac.mes = mes;
            SendMessage(GetDlgItem(hDlg, IDP_ANIO), WM_GETTEXT, sizeof(anioBuffer) / sizeof(anioBuffer[0]), (LPARAM)anioBuffer);
            anio = _wtoi(anioBuffer);
            fechaNac.anio = anio;
            SendMessage(GetDlgItem(hDlg, IDP_CIVIL), WM_GETTEXT, sizeof(estCivil) / sizeof(estCivil[0]), (LPARAM)estCivil);

            SendMessage(GetDlgItem(hDlg, IDP_CALLE), WM_GETTEXT, sizeof(calle) / sizeof(calle[0]), (LPARAM)calle);
            SendMessage(GetDlgItem(hDlg, IDP_COLONIA), WM_GETTEXT, sizeof(colonia) / sizeof(colonia[0]), (LPARAM)colonia);
            SendMessage(GetDlgItem(hDlg, IDP_CIUDAD), WM_GETTEXT, sizeof(ciudad) / sizeof(ciudad[0]), (LPARAM)ciudad);
            SendMessage(GetDlgItem(hDlg, IDP_ESTADO), WM_GETTEXT, sizeof(estado) / sizeof(estado[0]), (LPARAM)estado);
            SendMessage(GetDlgItem(hDlg, IDP_TEL), WM_GETTEXT, sizeof(phone) / sizeof(phone[0]), (LPARAM)phone);

            if (wcslen(apePatP) == 0 || wcslen(apeMatP) == 0 || wcslen(nombresP) == 0 || wcslen(curp) == 0 || wcslen(rfc) == 0 || wcslen(diaBuffer) == 0 || wcslen(mesBuffer) == 0 || wcslen(anioBuffer) == 0 || wcslen(estCivil) == 0
                || wcslen(calle) == 0 || wcslen(colonia) == 0 || wcslen(ciudad) == 0 || wcslen(estado) == 0 || wcslen(phone) == 0) {
                MessageBox(NULL, L"Por favor complete toda la información", L"AVISO", MB_ICONERROR);
                break;
            }
            for (int i = 0; i < wcslen(apePatP); i++) {
                if (!iswalpha(apePatP[i]) && (apePatP[i] != ' ')) {
                    MessageBox(NULL, L"El apellido paterno solo puede tener caracteres alfabéticos", L"AVISO", MB_ICONERROR);
                    return (INT_PTR)FALSE;
                }
            }
            for (int i = 0; i < wcslen(apeMatP); i++) {
                if (!iswalpha(apeMatP[i]) && (apeMatP[i] != ' ')) {
                    MessageBox(NULL, L"El apellido materno solo puede tener caracteres alfabéticos", L"AVISO", MB_ICONERROR);
                    return (INT_PTR)FALSE;
                }
            }
            for (int i = 0; i < wcslen(nombresP); i++) {
                if (!iswalpha(nombresP[i]) && (nombresP[i] != ' ')) {
                    MessageBox(NULL, L"El nombre solo puede tener caracteres alfabéticos", L"AVISO", MB_ICONERROR);
                    return (INT_PTR)FALSE;
                }
            }
            if (firstPer != NULL) {
                listCount = 0;
                listSize = myPeople.getSize();
                while (listCount < listSize) {
                    auxPer = myPeople.get(listCount);
                    if (wcscmp(auxPer->curp, curp) == 0) {
                        if (wcscmp(auxPer->curp, oldCURP) != 0) { // si cambio la clave, y esa clave nueva ya existia...
                            MessageBox(NULL, L"Ese CURP ya fue registrado.", L"AVISO", MB_ICONERROR);
                            return (INT_PTR)FALSE;
                        }
                    }
                    if (wcscmp(auxPer->rfc, rfc) == 0) {
                        if (wcscmp(auxPer->rfc, oldRFC) != 0) { // si cambio la clave, y esa clave nueva ya existia...
                            MessageBox(NULL, L"Ese RFC ya fue registrado.", L"AVISO", MB_ICONERROR);
                            return (INT_PTR)FALSE;
                        }
                    }
                    listCount++;
                }
            }
            auxPer = myPeople.get(globalSel--);
            editPersona(auxPer);
            MessageBox(NULL, L"¡La edición de la persona fue exitosa!", L"AVISO", MB_ICONINFORMATION);
            EndDialog(hDlg, LOWORD(wParam));
            if (!mainOpen) DialogBox(NULL, MAKEINTRESOURCE(IDD_MAIN), hDlg, MainMenu);
        }
        break;
    }
    case WM_CLOSE: {
        DialogBox(NULL, MAKEINTRESOURCE(IDD_CLOSE), hDlg, ExitCheck);
        if (closeWnd) {
            EndDialog(hDlg, LOWORD(wParam));
            if (!mainOpen) PostQuitMessage(0);
        }
        break;
    }
    default:
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK DeletePer(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) { // Para eliminar cualquier persona con un curp valido
    UNREFERENCED_PARAMETER(lParam);

    switch (message) {
    case WM_INITDIALOG: {
        SetDlgItemText(hDlg, IDP_SHOWUSER, nombreComp);
        fotoBM = NULL;
        if (firstPer != NULL) {
            listSize = myPeople.getSize();
            listCount = 0;
            while (listCount < listSize) { // muestra las claves de las vacunas registradas por el usuario actual
                auxPer = myPeople.get(listCount);
                if (auxPer->userID == logUser.userID) SendDlgItemMessage(hDlg, IDP_COMBO, CB_ADDSTRING, NULL, (LPARAM)auxPer->curp);
                listCount++;
            }
        }
        if (SendDlgItemMessage(hDlg, IDP_COMBO, CB_GETCOUNT, NULL, NULL) == 0) {
            MessageBox(NULL, L"No hay ninguna persona registrada que eliminar", L"AVISO", MB_ICONERROR);
            EndDialog(hDlg, LOWORD(wParam));
            if (!mainOpen) DialogBox(NULL, MAKEINTRESOURCE(IDD_MAIN), hDlg, MainMenu);
        }
        return (INT_PTR)TRUE;
    }
    case WM_COMMAND: {
        if (HIWORD(wParam) == CBN_SELENDOK) { // cuando el usuario seleccione una opcion del combo box
            int sel = SendDlgItemMessage(hDlg, IDP_COMBO, CB_GETCURSEL, NULL, NULL);
            SendDlgItemMessage(hDlg, IDP_COMBO, CB_GETLBTEXT, sel, (LPARAM)curp);
            listSize = myPeople.getSize();
            listCount = 0;
            foundPersona = false;
            while ((listCount < listSize) && !foundPersona) {
                auxPer = myPeople.get(listCount);
                if (auxPer->userID == logUser.userID) {
                    SendMessage(GetDlgItem(hDlg, IDP_COMBO), CB_GETLBTEXT, sizeof(curp) / sizeof(curp[0]), (LPARAM)curp);
                    if (wcscmp(auxPer->curp, curp) == 0) {
                        SetDlgItemText(hDlg, IDP_APEPAT, auxPer->apePat);
                        SetDlgItemText(hDlg, IDP_APEMAT, auxPer->apeMat);
                        SetDlgItemText(hDlg, IDP_NOMBRES, auxPer->nombres);
                        SetDlgItemText(hDlg, IDP_CURP, auxPer->curp);
                        SendMessage(GetDlgItem(hDlg, IDP_CURP), WM_GETTEXT, sizeof(oldCURP) / sizeof(oldCURP[0]), (LPARAM)oldCURP);
                        SetDlgItemText(hDlg, IDP_RFC, auxPer->rfc);
                        SendMessage(GetDlgItem(hDlg, IDP_RFC), WM_GETTEXT, sizeof(oldRFC) / sizeof(oldRFC[0]), (LPARAM)oldRFC);
                        swprintf_s(diaBuffer, 30, L"%d", auxPer->fechaNac.dia);
                        SetDlgItemText(hDlg, IDP_DIA, diaBuffer);
                        swprintf_s(mesBuffer, 30, L"%d", auxPer->fechaNac.mes);
                        SetDlgItemText(hDlg, IDP_MES, mesBuffer);
                        swprintf_s(anioBuffer, 30, L"%d", auxPer->fechaNac.anio);
                        SetDlgItemText(hDlg, IDP_ANIO, anioBuffer);
                        SetDlgItemText(hDlg, IDP_CIVIL, auxPer->estCivil);
                        SetDlgItemText(hDlg, IDP_CALLE, auxPer->calle);
                        SetDlgItemText(hDlg, IDP_COLONIA, auxPer->colonia);
                        SetDlgItemText(hDlg, IDP_CIUDAD, auxPer->ciudad);
                        SetDlgItemText(hDlg, IDP_ESTADO, auxPer->estado);
                        SetDlgItemText(hDlg, IDP_COLONIA, auxPer->colonia);
                        SetDlgItemText(hDlg, IDP_TEL, auxPer->phone);
                        SetDlgItemText(hDlg, IDP_ARCHIVO, auxPer->archivoID);
                        fotoBM = (HBITMAP)LoadImage(NULL, auxPer->foto, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
                        SendDlgItemMessage(hDlg, IDP_FOTO, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)fotoBM);
                        globalSel = listCount;
                        foundPersona = true;
                    }
                }
                listCount++;
            }
        }
        if (LOWORD(wParam) == IDP_DELETE) {
            myPeople.del(globalSel--);
            MessageBox(NULL, L"¡La eliminación de la persona fue exitosa!", L"AVISO", MB_ICONINFORMATION);
            EndDialog(hDlg, LOWORD(wParam));
            if (!mainOpen) DialogBox(NULL, MAKEINTRESOURCE(IDD_MAIN), hDlg, MainMenu);
        }
        break;
    }
    case WM_CLOSE: {
        DialogBox(NULL, MAKEINTRESOURCE(IDD_CLOSE), hDlg, ExitCheck);
        if (closeWnd) {
            EndDialog(hDlg, LOWORD(wParam));
            if (!mainOpen) PostQuitMessage(0);
        }
        break;
    }
    default:
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK RegVac(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);

    switch (message) {
    case WM_INITDIALOG: {
        SetDlgItemText(hDlg, IDV_SHOWUSER, nombreComp);
        return (INT_PTR)TRUE;
    }
    case WM_COMMAND: {
        if (LOWORD(wParam) == IDV_REG) {
            SendMessage(GetDlgItem(hDlg, IDV_TIPO), WM_GETTEXT, sizeof(tipo) / sizeof(tipo[0]), (LPARAM)tipo);
            SendMessage(GetDlgItem(hDlg, IDV_MARCA), WM_GETTEXT, sizeof(marca) / sizeof(marca[0]), (LPARAM)marca);
            SendMessage(GetDlgItem(hDlg, IDV_DOSIS), WM_GETTEXT, sizeof(dosisBuffer) / sizeof(dosisBuffer[0]), (LPARAM)dosisBuffer);
            dosis = _wtoi(dosisBuffer);
            SendMessage(GetDlgItem(hDlg, IDV_CLAVE), WM_GETTEXT, sizeof(clave) / sizeof(clave[0]), (LPARAM)clave);
            SendMessage(GetDlgItem(hDlg, IDV_DESC), WM_GETTEXT, sizeof(desc) / sizeof(desc[0]), (LPARAM)desc);
            SendMessage(GetDlgItem(hDlg, IDV_PRECIO), WM_GETTEXT, sizeof(precioBuffer) / sizeof(precioBuffer[0]), (LPARAM)precioBuffer);
            precio = _wtof(precioBuffer);
            precio = floorf(precio * 100) / 100;
            swprintf_s(precioBuffer, 30, L"%.2f", precio);

            if (wcslen(tipo) == 0 || wcslen(marca) == 0 || dosis == 0 || wcslen(clave) == 0 || wcslen(desc) == 0) {
                MessageBox(NULL, L"Por favor complete toda la información", L"AVISO", MB_ICONERROR);
                break;
            }
            if (dosis <= 0) {
                MessageBox(NULL, L"El numero de dosis debe ser mayor a 0.", L"AVISO", MB_ICONERROR);
                break;
            }
            if (precio <= 0) {
                MessageBox(NULL, L"El precio debe ser mayor que cero.", L"AVISO", MB_ICONERROR);
                break;
            }
            if (firstVac != NULL) {
                listCount = 0;
                listSize = myVaccines.getSize();
                while (listCount < listSize) {
                    auxVac = myVaccines.get(listCount);
                    if (wcscmp(auxVac->clave, clave) == 0) {
                        MessageBox(NULL, L"Esa clave de vacuna ya fue registrada.", L"AVISO", MB_ICONERROR);
                        return (INT_PTR)FALSE;
                    }
                    listCount++;
                }
            }
            saveVacuna();
            MessageBox(NULL, L"¡El registro de la vacuna fue exitoso!", L"AVISO", MB_ICONINFORMATION);
            EndDialog(hDlg, LOWORD(wParam));
            if (!mainOpen) DialogBox(NULL, MAKEINTRESOURCE(IDD_MAIN), hDlg, MainMenu);
        }
        break;
    }
    case WM_CLOSE: {
        DialogBox(NULL, MAKEINTRESOURCE(IDD_CLOSE), hDlg, ExitCheck);
        if (closeWnd) {
            EndDialog(hDlg, LOWORD(wParam));
            if (!mainOpen) PostQuitMessage(0);
        }
        break;
    }
    default:
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK EditVac(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) { // Para editar cualquier vacuna con una clave valida
    UNREFERENCED_PARAMETER(lParam);

    switch (message) {
    case WM_INITDIALOG: {
        SetDlgItemText(hDlg, IDV_SHOWUSER, nombreComp);
        if (firstVac != NULL) {
            listSize = myVaccines.getSize();
            listCount = 0;
            while (listCount < listSize) { // muestra las claves de las vacunas registradas por el usuario actual
                auxVac = myVaccines.get(listCount);
                if (auxVac->userID == logUser.userID) SendDlgItemMessage(hDlg, IDV_COMBO, CB_ADDSTRING, NULL, (LPARAM)auxVac->clave);
                listCount++;
            }
        }
        if (SendDlgItemMessage(hDlg, IDV_COMBO, CB_GETCOUNT, NULL, NULL) == 0) {
            MessageBox(NULL, L"No hay ninguna vacuna registrada que editar", L"AVISO", MB_ICONERROR);
            EndDialog(hDlg, LOWORD(wParam));
            if (!mainOpen) DialogBox(NULL, MAKEINTRESOURCE(IDD_MAIN), hDlg, MainMenu);
        }
        return (INT_PTR)TRUE;
    }
    case WM_COMMAND: {
        if (HIWORD(wParam) == CBN_SELENDOK) { // cuando el usuario seleccione una opcion del combo box
            int sel = SendDlgItemMessage(hDlg, IDV_COMBO, CB_GETCURSEL, NULL, NULL);
            SendDlgItemMessage(hDlg, IDV_COMBO, CB_GETLBTEXT, sel, (LPARAM)clave);
            listSize = myVaccines.getSize();
            listCount = 0;
            foundVac = false;
            while ((listCount < listSize) && !foundVac) { // busca por todas las claves hasta encontrar la deseada
                auxVac = myVaccines.get(listCount);
                if (auxVac->userID == logUser.userID) {
                    SendMessage(GetDlgItem(hDlg, IDV_COMBO), CB_GETLBTEXT, sizeof(clave) / sizeof(clave[0]), (LPARAM)clave);
                    if (wcscmp(auxVac->clave, clave) == 0) { // si se encontro la clave registrada con el usuario, despliega sus datos
                        SetDlgItemText(hDlg, IDV_TIPO, auxVac->tipo);
                        SetDlgItemText(hDlg, IDV_MARCA, auxVac->marca);
                        swprintf_s(dosisBuffer, 30, L"%d", auxVac->dosis);
                        SetDlgItemText(hDlg, IDV_DOSIS, dosisBuffer);
                        SetDlgItemText(hDlg, IDV_CLAVE, auxVac->clave);
                        SendMessage(GetDlgItem(hDlg, IDV_CLAVE), WM_GETTEXT, sizeof(oldClave) / sizeof(oldClave[0]), (LPARAM)oldClave);
                        SetDlgItemText(hDlg, IDV_DESC, auxVac->desc);
                        swprintf_s(precioBuffer, 30, L"%.2f", auxVac->precio);
                        SetDlgItemText(hDlg, IDV_PRECIO, precioBuffer);
                        globalSel = listCount;
                        foundVac = true;
                    }
                }
                listCount++;
            }
        }
        if (LOWORD(wParam) == IDV_SAVE) {
            SendMessage(GetDlgItem(hDlg, IDV_TIPO), WM_GETTEXT, sizeof(tipo) / sizeof(tipo[0]), (LPARAM)tipo);
            SendMessage(GetDlgItem(hDlg, IDV_MARCA), WM_GETTEXT, sizeof(marca) / sizeof(marca[0]), (LPARAM)marca);
            SendMessage(GetDlgItem(hDlg, IDV_DOSIS), WM_GETTEXT, sizeof(dosisBuffer) / sizeof(dosisBuffer[0]), (LPARAM)dosisBuffer);
            dosis = _wtoi(dosisBuffer);
            SendMessage(GetDlgItem(hDlg, IDV_CLAVE), WM_GETTEXT, sizeof(clave) / sizeof(clave[0]), (LPARAM)clave);
            SendMessage(GetDlgItem(hDlg, IDV_DESC), WM_GETTEXT, sizeof(desc) / sizeof(desc[0]), (LPARAM)desc);
            SendMessage(GetDlgItem(hDlg, IDV_PRECIO), WM_GETTEXT, sizeof(precioBuffer) / sizeof(precioBuffer[0]), (LPARAM)precioBuffer);
            precio = _wtof(precioBuffer);
            precio = floorf(precio * 100) / 100;
            swprintf_s(precioBuffer, 30, L"%.2f", precio);

            if (wcslen(tipo) == 0 || wcslen(marca) == 0 || dosis == 0 || wcslen(clave) == 0 || wcslen(desc) == 0) {
                MessageBox(NULL, L"Por favor complete toda la información", L"AVISO", MB_ICONERROR);
                break;
            }
            if (dosis <= 0) {
                MessageBox(NULL, L"El numero de dosis debe ser mayor a 0.", L"AVISO", MB_ICONERROR);
                break;
            }
            if (precio <= 0) {
                MessageBox(NULL, L"El precio debe ser mayor que cero.", L"AVISO", MB_ICONERROR);
                break;
            }
            if (firstVac != NULL) {
                listCount = 0;
                listSize = myVaccines.getSize();
                while (listCount < listSize) {
                    auxVac = myVaccines.get(listCount);
                    if (wcscmp(auxVac->clave, clave) == 0) {
                        if (wcscmp(auxVac->clave, oldClave) != 0) { // si cambio la clave, y esa clave nueva ya existia...
                            MessageBox(NULL, L"Esa clave de vacuna ya fue registrada.", L"AVISO", MB_ICONERROR);
                            return (INT_PTR)FALSE;
                        }
                    }
                    listCount++;
                }
            }
            auxVac = myVaccines.get(globalSel--);
            editVacuna(auxVac);
            MessageBox(NULL, L"¡La edición de la vacuna fue exitosa!", L"AVISO", MB_ICONINFORMATION);
            EndDialog(hDlg, LOWORD(wParam));
            if (!mainOpen) DialogBox(NULL, MAKEINTRESOURCE(IDD_MAIN), hDlg, MainMenu);
        }
        break;
    }
    case WM_CLOSE: {
        DialogBox(NULL, MAKEINTRESOURCE(IDD_CLOSE), hDlg, ExitCheck);
        if (closeWnd) {
            EndDialog(hDlg, LOWORD(wParam));
            if (!mainOpen) PostQuitMessage(0);
        }
        break;
    }
    default:
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK DeleteVac(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) { // Para eliminar cualquier vacuna con una clave valida
    UNREFERENCED_PARAMETER(lParam);

    switch (message) {
    case WM_INITDIALOG: {
        SetDlgItemText(hDlg, IDV_SHOWUSER, nombreComp);
        if (firstVac != NULL) {
            listSize = myVaccines.getSize();
            listCount = 0;
            while (listCount < listSize) { // muestra las claves de las vacunas registradas por el usuario actual
                auxVac = myVaccines.get(listCount);
                if (auxVac->userID == logUser.userID) SendDlgItemMessage(hDlg, IDV_COMBO, CB_ADDSTRING, NULL, (LPARAM)auxVac->clave);
                listCount++;
            }
        }
        if (SendDlgItemMessage(hDlg, IDV_COMBO, CB_GETCOUNT, NULL, NULL) == 0) {
            MessageBox(NULL, L"No hay ninguna vacuna registrada que eliminar", L"AVISO", MB_ICONERROR);
            EndDialog(hDlg, LOWORD(wParam));
            if (!mainOpen) DialogBox(NULL, MAKEINTRESOURCE(IDD_MAIN), hDlg, MainMenu);
        }
        return (INT_PTR)TRUE;
    }
    case WM_COMMAND: {
        if (HIWORD(wParam) == CBN_SELENDOK) { // cuando el usuario seleccione una opcion del combo box
            int sel = SendDlgItemMessage(hDlg, IDV_COMBO, CB_GETCURSEL, NULL, NULL);
            SendDlgItemMessage(hDlg, IDV_COMBO, CB_GETLBTEXT, sel, (LPARAM)clave);
            listSize = myVaccines.getSize();
            listCount = 0;
            foundVac = false;
            while ((listCount < listSize) && !foundVac) { // busca por todas las claves hasta encontrar la deseada
                auxVac = myVaccines.get(listCount);
                if (auxVac->userID == logUser.userID) {
                    SendMessage(GetDlgItem(hDlg, IDV_COMBO), CB_GETLBTEXT, sizeof(clave) / sizeof(clave[0]), (LPARAM)clave);
                    if (wcscmp(auxVac->clave, clave) == 0) { // si se encontro la clave registrada con el usuario, despliega sus datos
                        SetDlgItemText(hDlg, IDV_TIPO, auxVac->tipo);
                        SetDlgItemText(hDlg, IDV_MARCA, auxVac->marca);
                        swprintf_s(dosisBuffer, 30, L"%d", auxVac->dosis);
                        SetDlgItemText(hDlg, IDV_DOSIS, dosisBuffer);
                        SetDlgItemText(hDlg, IDV_CLAVE, auxVac->clave);
                        SendMessage(GetDlgItem(hDlg, IDV_CLAVE), WM_GETTEXT, sizeof(oldClave) / sizeof(oldClave[0]), (LPARAM)oldClave);
                        SetDlgItemText(hDlg, IDV_DESC, auxVac->desc);
                        swprintf_s(precioBuffer, 30, L"%.2f", auxVac->precio);
                        SetDlgItemText(hDlg, IDV_PRECIO, precioBuffer);
                        globalSel = listCount;
                        foundVac = true;
                    }
                }
                listCount++;
            }
        }
        if (LOWORD(wParam) == IDV_DELETE) {
            myVaccines.del(globalSel--);
            MessageBox(NULL, L"¡La eliminación de la vacuna fue exitosa!", L"AVISO", MB_ICONINFORMATION);
            EndDialog(hDlg, LOWORD(wParam));
            if (!mainOpen) DialogBox(NULL, MAKEINTRESOURCE(IDD_MAIN), hDlg, MainMenu);
        }
        break;
    }
    case WM_CLOSE: {
        DialogBox(NULL, MAKEINTRESOURCE(IDD_CLOSE), hDlg, ExitCheck);
        if (closeWnd) {
            EndDialog(hDlg, LOWORD(wParam));
            if (!mainOpen) PostQuitMessage(0);
        }
        break;
    }
    default:
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK RegCarne(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);

    switch (message) {
    case WM_INITDIALOG: {
        personaSel = -1;
        vacunaSel = -1;
        SetDlgItemText(hDlg, IDC_SHOWUSER, nombreComp);
        if (firstPer != NULL) {
            listSize = myPeople.getSize();
            listCount = 0;
            while (listCount < listSize) { // muestra las claves de las vacunas registradas por el usuario actual
                auxPer = myPeople.get(listCount);
                if (auxPer->userID == logUser.userID) SendDlgItemMessage(hDlg, IDP_COMBO, CB_ADDSTRING, NULL, (LPARAM)auxPer->curp);
                listCount++;
            }
        }
        if (SendDlgItemMessage(hDlg, IDP_COMBO, CB_GETCOUNT, NULL, NULL) == 0) {
            MessageBox(NULL, L"No hay ninguna persona registrada que eliminar", L"AVISO", MB_ICONERROR);
            EndDialog(hDlg, LOWORD(wParam));
            if (!mainOpen) DialogBox(NULL, MAKEINTRESOURCE(IDD_MAIN), hDlg, MainMenu);
        }
        if (firstVac != NULL) {
            listSize = myVaccines.getSize();
            listCount = 0;
            while (listCount < listSize) { // muestra las claves de las vacunas registradas por el usuario actual
                auxVac = myVaccines.get(listCount);
                if (auxVac->userID == logUser.userID) SendDlgItemMessage(hDlg, IDV_COMBO, CB_ADDSTRING, NULL, (LPARAM)auxVac->clave);
                listCount++;
            }
        }
        if (SendDlgItemMessage(hDlg, IDV_COMBO, CB_GETCOUNT, NULL, NULL) == 0) {
            MessageBox(NULL, L"No hay ninguna vacuna registrada que editar", L"AVISO", MB_ICONERROR);
            EndDialog(hDlg, LOWORD(wParam));
            if (!mainOpen) DialogBox(NULL, MAKEINTRESOURCE(IDD_MAIN), hDlg, MainMenu);
        }
        return (INT_PTR)TRUE;
    }
    case WM_COMMAND: {
        if (HIWORD(wParam) == CBN_SELENDOK) { // cuando el usuario seleccione una opcion del combo box
            if (LOWORD(wParam) == IDP_COMBO) {
                int sel = SendDlgItemMessage(hDlg, IDP_COMBO, CB_GETCURSEL, NULL, NULL);
                SendDlgItemMessage(hDlg, IDP_COMBO, CB_GETLBTEXT, sel, (LPARAM)curp);
                listSize = myPeople.getSize();
                listCount = 0;
                foundPersona = false;
                while ((listCount < listSize) && !foundPersona) {
                    auxPer = myPeople.get(listCount);
                    if (auxPer->userID == logUser.userID) {
                        SendMessage(GetDlgItem(hDlg, IDP_COMBO), CB_GETLBTEXT, sizeof(curp) / sizeof(curp[0]), (LPARAM)curp);
                        if (wcscmp(auxPer->curp, curp) == 0) {
                            SetDlgItemText(hDlg, IDP_APEPAT, auxPer->apePat);
                            SetDlgItemText(hDlg, IDP_APEMAT, auxPer->apeMat);
                            SetDlgItemText(hDlg, IDP_NOMBRES, auxPer->nombres);
                            SetDlgItemText(hDlg, IDP_RFC, auxPer->rfc);
                            swprintf_s(diaBuffer, 30, L"%d", auxPer->fechaNac.dia);
                            SetDlgItemText(hDlg, IDP_DIA, diaBuffer);
                            swprintf_s(mesBuffer, 30, L"%d", auxPer->fechaNac.mes);
                            SetDlgItemText(hDlg, IDP_MES, mesBuffer);
                            swprintf_s(anioBuffer, 30, L"%d", auxPer->fechaNac.anio);
                            SetDlgItemText(hDlg, IDP_ANIO, anioBuffer);
                            SetDlgItemText(hDlg, IDP_CIVIL, auxPer->estCivil);
                            SetDlgItemText(hDlg, IDP_CALLE, auxPer->calle);
                            SetDlgItemText(hDlg, IDP_COLONIA, auxPer->colonia);
                            SetDlgItemText(hDlg, IDP_CIUDAD, auxPer->ciudad);
                            SetDlgItemText(hDlg, IDP_ESTADO, auxPer->estado);
                            SetDlgItemText(hDlg, IDP_COLONIA, auxPer->colonia);
                            SetDlgItemText(hDlg, IDP_TEL, auxPer->phone);
                            SetDlgItemText(hDlg, IDP_ARCHIVO, auxPer->archivoID);
                            fotoBM = (HBITMAP)LoadImage(NULL, auxPer->foto, IMAGE_BITMAP, 210, 210, LR_LOADFROMFILE);
                            SendDlgItemMessage(hDlg, IDP_FOTO, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)fotoBM);
                            personaSel = listCount;
                            foundPersona = true;
                        }
                    }
                    listCount++;
                }
            }
            if (LOWORD(wParam) == IDV_COMBO) {
                int sel = SendDlgItemMessage(hDlg, IDV_COMBO, CB_GETCURSEL, NULL, NULL);
                SendDlgItemMessage(hDlg, IDV_COMBO, CB_GETLBTEXT, sel, (LPARAM)clave);
                listSize = myVaccines.getSize();
                listCount = 0;
                foundVac = false;
                while ((listCount < listSize) && !foundVac) { // busca por todas las claves hasta encontrar la deseada
                    auxVac = myVaccines.get(listCount);
                    if (auxVac->userID == logUser.userID) {
                        SendMessage(GetDlgItem(hDlg, IDV_COMBO), CB_GETLBTEXT, sizeof(clave) / sizeof(clave[0]), (LPARAM)clave);
                        if (wcscmp(auxVac->clave, clave) == 0) { // si se encontro la clave registrada con el usuario, despliega sus datos
                            SetDlgItemText(hDlg, IDV_TIPO, auxVac->tipo);
                            SetDlgItemText(hDlg, IDV_MARCA, auxVac->marca);
                            swprintf_s(dosisBuffer, 30, L"%d", auxVac->dosis);
                            SetDlgItemText(hDlg, IDV_DOSIS, dosisBuffer);
                            SetDlgItemText(hDlg, IDV_DESC, auxVac->desc);
                            swprintf_s(precioBuffer, 30, L"%.2f", auxVac->precio);
                            SetDlgItemText(hDlg, IDV_PRECIO, precioBuffer);
                            // limpia la fecha de sig dosis si se cambia la vacuna
                            swprintf_s(dia3Buffer, 30, L"%d", NULL);
                            SetDlgItemText(hDlg, IDC_DIASIG, dia3Buffer);
                            swprintf_s(mes3Buffer, 30, L"%d", NULL);
                            SetDlgItemText(hDlg, IDC_MESSIG, mes3Buffer);
                            swprintf_s(mes3Buffer, 30, L"%d", NULL);
                            SetDlgItemText(hDlg, IDC_ANIOSIG, mes3Buffer);
                            //
                            vacunaSel = listCount;
                            foundVac = true;
                        }
                    }
                    listCount++;
                }
            }
        }
        if (LOWORD(wParam) == IDC_SIGDOSIS) { // boton para calcular la fecha de la sig dosis
            #pragma region prevencion de errores
            SendMessage(GetDlgItem(hDlg, IDC_DOSIS), WM_GETTEXT, sizeof(numDosisBuffer) / sizeof(numDosisBuffer[0]), (LPARAM)numDosisBuffer);
            numDosis = _wtoi(numDosisBuffer);
            if (wcslen(numDosisBuffer) == 0) {
                MessageBox(NULL, L"Por favor llene el campo 'Dosis Actual' primero.", L"AVISO", MB_ICONERROR);
                break;
            }
            if (numDosis <= 0) {
                MessageBox(NULL, L"Su dosis actual debe ser mayor a 0.", L"AVISO", MB_ICONERROR);
                break;
            }
            auxVac = myVaccines.get(vacunaSel);
            if (numDosis > auxVac->dosis) {
                MessageBox(NULL, L"Su dosis actual no puede ser mayor al que tiene la vacuna que selecciono.", L"AVISO", MB_ICONERROR);
                break;
            }
            if (numDosis == auxVac->dosis) {
                MessageBox(NULL, L"Esta persona ya no tiene dosis pendientes. No es necesario calcular.", L"AVISO", MB_ICONERROR);
                break;
            }
            SendMessage(GetDlgItem(hDlg, IDC_DIASDOSIS), WM_GETTEXT, sizeof(diasDosisBuffer) / sizeof(diasDosisBuffer[0]), (LPARAM)diasDosisBuffer);
            diasEntreDosis = _wtoi(diasDosisBuffer);
            if (wcslen(diasDosisBuffer) == 0) {
                MessageBox(NULL, L"Por favor llene el campo 'Días a esperar' primero.", L"AVISO", MB_ICONERROR);
                break;
            }
            if (diasEntreDosis <= 0) {
                MessageBox(NULL, L"Los días a esperar antes de la siguiente dosis deben ser mayor a 0.", L"AVISO", MB_ICONERROR);
                break;
            }
            SendMessage(GetDlgItem(hDlg, IDC_DIA), WM_GETTEXT, sizeof(dia2Buffer) / sizeof(dia2Buffer[0]), (LPARAM)dia2Buffer);
            dia = _wtoi(dia2Buffer);
            fechaDosis.dia = dia;
            SendMessage(GetDlgItem(hDlg, IDC_MES), WM_GETTEXT, sizeof(mes2Buffer) / sizeof(mes2Buffer[0]), (LPARAM)mes2Buffer);
            mes = _wtoi(mes2Buffer);
            fechaDosis.mes = mes;
            SendMessage(GetDlgItem(hDlg, IDC_ANIO), WM_GETTEXT, sizeof(anio2Buffer) / sizeof(anio2Buffer[0]), (LPARAM)anio2Buffer);
            anio = _wtoi(anio2Buffer);
            fechaDosis.anio = anio;
            if (wcslen(dia2Buffer) == 0 || wcslen(mes2Buffer) == 0 || wcslen(anio2Buffer) == 0) {
                MessageBox(NULL, L"Por favor llene todos los campos de 'Fecha de esta dosis' primero.", L"AVISO", MB_ICONERROR);
                break;
            }
            #pragma endregion
            sumFecha(fechaDosis, diasEntreDosis, sigDosis);
            swprintf_s(dia3Buffer, 30, L"%d", sigDosis.dia);
            SetDlgItemText(hDlg, IDC_DIASIG, dia3Buffer);
            swprintf_s(mes3Buffer, 30, L"%d", sigDosis.mes);
            SetDlgItemText(hDlg, IDC_MESSIG, mes3Buffer);
            swprintf_s(anio3Buffer, 30, L"%d", sigDosis.anio);
            SetDlgItemText(hDlg, IDC_ANIOSIG, anio3Buffer);
        }
        if (LOWORD(wParam) == IDC_REG) {
            auxPer = myPeople.get(personaSel);
            auxVac = myVaccines.get(vacunaSel);
            SendMessage(GetDlgItem(hDlg, IDC_DOSIS), WM_GETTEXT, sizeof(numDosisBuffer) / sizeof(numDosisBuffer[0]), (LPARAM)numDosisBuffer);
            numDosis = _wtoi(numDosisBuffer);
            SendMessage(GetDlgItem(hDlg, IDC_DIA), WM_GETTEXT, sizeof(dia2Buffer) / sizeof(dia2Buffer[0]), (LPARAM)dia2Buffer);
            dia = _wtoi(dia2Buffer);
            fechaDosis.dia = dia;
            SendMessage(GetDlgItem(hDlg, IDC_MES), WM_GETTEXT, sizeof(mes2Buffer) / sizeof(mes2Buffer[0]), (LPARAM)mes2Buffer);
            mes = _wtoi(mes2Buffer);
            fechaDosis.mes = mes;
            SendMessage(GetDlgItem(hDlg, IDC_ANIO), WM_GETTEXT, sizeof(anio2Buffer) / sizeof(anio2Buffer[0]), (LPARAM)anio2Buffer);
            anio = _wtoi(anio2Buffer);
            fechaDosis.anio = anio;
            SendMessage(GetDlgItem(hDlg, IDC_DIASDOSIS), WM_GETTEXT, sizeof(diasDosisBuffer) / sizeof(diasDosisBuffer[0]), (LPARAM)diasDosisBuffer);
            diasEntreDosis = _wtoi(diasDosisBuffer);
            SendMessage(GetDlgItem(hDlg, IDC_DIASIG), WM_GETTEXT, sizeof(dia3Buffer) / sizeof(dia3Buffer[0]), (LPARAM)dia3Buffer);
            dia = _wtoi(dia3Buffer);
            sigDosis.dia = dia;
            SendMessage(GetDlgItem(hDlg, IDC_MESSIG), WM_GETTEXT, sizeof(mes3Buffer) / sizeof(mes3Buffer[0]), (LPARAM)mes3Buffer);
            mes = _wtoi(mes3Buffer);
            sigDosis.mes = mes;
            SendMessage(GetDlgItem(hDlg, IDC_ANIOSIG), WM_GETTEXT, sizeof(anio3Buffer) / sizeof(anio3Buffer[0]), (LPARAM)anio3Buffer);
            anio = _wtoi(anio3Buffer);
            sigDosis.anio = anio;
            SendMessage(GetDlgItem(hDlg, IDC_LOTE), WM_GETTEXT, sizeof(lote) / sizeof(lote[0]), (LPARAM)lote);
            SendMessage(GetDlgItem(hDlg, IDC_CENTRO), WM_GETTEXT, sizeof(centroVacu) / sizeof(centroVacu[0]), (LPARAM)centroVacu);

            if (personaSel == -1 || vacunaSel == -1 || wcslen(numDosisBuffer) == 0 || wcslen(dia2Buffer) == 0 || wcslen(mes2Buffer) == 0 || 
                wcslen(anio2Buffer) == 0 || wcslen(lote) == 0 || wcslen(centroVacu) == 0) {
                MessageBox(NULL, L"Por favor complete toda la información", L"AVISO", MB_ICONERROR);
                break;
            }
            if (numDosis <= 0) {
                MessageBox(NULL, L"Su dosis actual debe ser mayor a 0.", L"AVISO", MB_ICONERROR);
                break;
            }
            if (numDosis > auxVac->dosis) {
                MessageBox(NULL, L"Su dosis actual no puede ser mayor al que tiene la vacuna que selecciono.", L"AVISO", MB_ICONERROR);
                break;
            }
            // si la persona tiene otra dosis pendiente, no puede registrar el carne hasta haber calculado la proxima fecha
            if ((numDosis < auxVac->dosis) && (wcslen(diasDosisBuffer) == 0 || wcslen(dia3Buffer) == 0 || wcslen(mes3Buffer) == 0 
                || wcslen(anio3Buffer) == 0 || diasEntreDosis == 0 || dia == 0 || mes == 0 || anio == 0)) {
                MessageBox(NULL, L"Por favor calcule la fecha de su próxima dosis.", L"AVISO", MB_ICONERROR);
                break;
            }
            // si lo anterior no aplica, significa que esta en su ultima dosis y estos datos los dejamos en 0
            if (diasEntreDosis == 0 || dia == 0 || mes == 0 || anio == 0) {
                diasEntreDosis = 0;
                sigDosis.dia = 0;
                sigDosis.mes = 0;
                sigDosis.anio = 0;
            }

            // Algoritmo para generar un número aleatorio de 4 dígitos. Sigue corriendo en caso de que la cifra que salga ya se haya registrado
            // Basado en el código recuperado de: https://en.cppreference.com/w/cpp/numeric/random/rand
            srand((unsigned)time(0));
            do {
                carnetID = 0;
                for (int n = 0; n != 4; ++n) {
                    int x = 10;
                    while (x > 9) {
                        x = rand() / ((RAND_MAX + 1u) / 9);
                    }
                    switch (n) {
                    case 0:
                        if (x == 0) x++; // para evitar que solo me de 3 numeros al final
                        carnetID += (x * 1000);
                        break;
                    case 1:
                        carnetID += (x * 100);
                        break;
                    case 2:
                        carnetID += (x * 10);
                        break;
                    case 3:
                        carnetID += x;
                        break;
                    default:
                        break;
                    }
                }
            } while (myCarnes.binarySearch(carnetID) != NULL);
            int n = myCarnes.getSize();
            saveCarne();
            MessageBox(NULL, L"¡El registro del carné fue exitoso!", L"AVISO", MB_ICONINFORMATION);
            EndDialog(hDlg, LOWORD(wParam));
            if (!mainOpen) DialogBox(NULL, MAKEINTRESOURCE(IDD_MAIN), hDlg, MainMenu);
        }
        break;
    }
    case WM_CLOSE: {
        DialogBox(NULL, MAKEINTRESOURCE(IDD_CLOSE), hDlg, ExitCheck);
        if (closeWnd) {
            EndDialog(hDlg, LOWORD(wParam));
            if (!mainOpen) PostQuitMessage(0);
        }
        break;
    }
    default:
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK EditCarne(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) { // Para editar cualquier carne
    UNREFERENCED_PARAMETER(lParam);

    switch (message) {
    case WM_INITDIALOG: {
        SetDlgItemText(hDlg, IDC_SHOWUSER, nombreComp);
        if (firstCarne != NULL) {
            listSize = myCarnes.getSize();
            listCount = 0;
            while (listCount < listSize) { // muestra los numeros unicos de carné registrados
                auxCarne = myCarnes.get(listCount);
                swprintf_s(carnetIDBuffer, 30, L"%d", auxCarne->carnetID);
                if (auxCarne->userID == logUser.userID) {
                    SendDlgItemMessage(hDlg, IDC_COMBONUM, CB_ADDSTRING, NULL, (LPARAM)carnetIDBuffer);
                }
                listCount++;
            }
        }
        if (SendDlgItemMessage(hDlg, IDC_COMBONUM, CB_GETCOUNT, NULL, NULL) == 0) {
            MessageBox(NULL, L"No hay ningun carné registrado que editar", L"AVISO", MB_ICONERROR);
            EndDialog(hDlg, LOWORD(wParam));
            if (!mainOpen) DialogBox(NULL, MAKEINTRESOURCE(IDD_MAIN), hDlg, MainMenu);
        }
        return (INT_PTR)TRUE;
    }
    case WM_COMMAND: {
        if (HIWORD(wParam) == CBN_SELENDOK) {
            // busca usando busqueda binaria
            // completa
            if (LOWORD(wParam) == IDC_COMBONUM) {
                int sel = SendDlgItemMessage(hDlg, IDC_COMBONUM, CB_GETCURSEL, NULL, NULL);
                SendDlgItemMessage(hDlg, IDC_COMBONUM, CB_GETLBTEXT, sel, (LPARAM)carnetIDBuffer);
                ///////
                carnetID = _wtoi(carnetIDBuffer);
                auxCarne = myCarnes.binarySearch(carnetID);
                ///////
                #pragma region persona get
                auxPer = auxCarne->persona;
                SetDlgItemText(hDlg, IDP_APEPAT, auxPer->apePat);
                SetDlgItemText(hDlg, IDP_APEMAT, auxPer->apeMat);
                SetDlgItemText(hDlg, IDP_NOMBRES, auxPer->nombres);
                SetDlgItemText(hDlg, IDP_CURP, auxPer->curp);
                SetDlgItemText(hDlg, IDP_RFC, auxPer->rfc);
                swprintf_s(diaBuffer, 30, L"%d", auxPer->fechaNac.dia);
                SetDlgItemText(hDlg, IDP_DIA, diaBuffer);
                swprintf_s(mesBuffer, 30, L"%d", auxPer->fechaNac.mes);
                SetDlgItemText(hDlg, IDP_MES, mesBuffer);
                swprintf_s(anioBuffer, 30, L"%d", auxPer->fechaNac.anio);
                SetDlgItemText(hDlg, IDP_ANIO, anioBuffer);
                SetDlgItemText(hDlg, IDP_CIVIL, auxPer->estCivil);
                SetDlgItemText(hDlg, IDP_CALLE, auxPer->calle);
                SetDlgItemText(hDlg, IDP_COLONIA, auxPer->colonia);
                SetDlgItemText(hDlg, IDP_CIUDAD, auxPer->ciudad);
                SetDlgItemText(hDlg, IDP_ESTADO, auxPer->estado);
                SetDlgItemText(hDlg, IDP_COLONIA, auxPer->colonia);
                SetDlgItemText(hDlg, IDP_TEL, auxPer->phone);
                SetDlgItemText(hDlg, IDP_ARCHIVO, auxPer->archivoID);
                fotoBM = (HBITMAP)LoadImage(NULL, auxPer->foto, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
                SendDlgItemMessage(hDlg, IDP_FOTO, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)fotoBM);
                #pragma endregion
                ///////
                #pragma region vacuna get
                auxVac = auxCarne->vacuna;
                SetDlgItemText(hDlg, IDV_TIPO, auxVac->tipo);
                SetDlgItemText(hDlg, IDV_MARCA, auxVac->marca);
                SetDlgItemText(hDlg, IDV_CLAVE2, auxVac->clave);
                swprintf_s(dosisBuffer, 30, L"%d", auxVac->dosis);
                SetDlgItemText(hDlg, IDV_DOSIS, dosisBuffer);
                SetDlgItemText(hDlg, IDV_DESC, auxVac->desc);
                swprintf_s(precioBuffer, 30, L"%.2f", auxVac->precio);
                SetDlgItemText(hDlg, IDV_PRECIO, precioBuffer);
                #pragma endregion
                ///////
                swprintf_s(numDosisBuffer, 30, L"%d", auxCarne->numDosis);
                SetDlgItemText(hDlg, IDC_DOSIS, numDosisBuffer);
                swprintf_s(dia2Buffer, 30, L"%d", auxCarne->fechaDosis.dia);
                SetDlgItemText(hDlg, IDC_DIA, dia2Buffer);
                swprintf_s(mes2Buffer, 30, L"%d", auxCarne->fechaDosis.mes);
                SetDlgItemText(hDlg, IDC_MES, mes2Buffer);
                swprintf_s(anio2Buffer, 30, L"%d", auxCarne->fechaDosis.anio);
                SetDlgItemText(hDlg, IDC_ANIO, anio2Buffer);
                swprintf_s(diasDosisBuffer, 30, L"%d", auxCarne->diasEntreDosis);
                SetDlgItemText(hDlg, IDC_DIASDOSIS, diasDosisBuffer);
                swprintf_s(dia3Buffer, 30, L"%d", auxCarne->sigDosis.dia);
                SetDlgItemText(hDlg, IDC_DIASIG, dia3Buffer);
                swprintf_s(mes3Buffer, 30, L"%d", auxCarne->sigDosis.mes);
                SetDlgItemText(hDlg, IDC_MESSIG, mes3Buffer);
                swprintf_s(anio3Buffer, 30, L"%d", auxCarne->sigDosis.anio);
                SetDlgItemText(hDlg, IDC_ANIOSIG, anio3Buffer);
                swprintf_s(diasDosisBuffer, 30, L"%d", auxCarne->diasEntreDosis);
                SetDlgItemText(hDlg, IDC_LOTE, auxCarne->lote);
                SetDlgItemText(hDlg, IDC_CENTRO, auxCarne->centroVacu);
            }
        }
        if (LOWORD(wParam) == IDC_SIGDOSIS) { // boton para calcular la fecha de la sig dosis
            #pragma region prevencion de errores
            SendMessage(GetDlgItem(hDlg, IDC_DOSIS), WM_GETTEXT, sizeof(numDosisBuffer) / sizeof(numDosisBuffer[0]), (LPARAM)numDosisBuffer);
            numDosis = _wtoi(numDosisBuffer);
            if (wcslen(numDosisBuffer) == 0) {
                MessageBox(NULL, L"Por favor llene el campo 'Dosis Actual' primero.", L"AVISO", MB_ICONERROR);
                break;
            }
            if (numDosis <= 0) {
                MessageBox(NULL, L"Su dosis actual debe ser mayor a 0.", L"AVISO", MB_ICONERROR);
                break;
            }
            auxVac = myVaccines.get(vacunaSel);
            if (numDosis > auxVac->dosis) {
                MessageBox(NULL, L"Su dosis actual no puede ser mayor al que tiene la vacuna que selecciono.", L"AVISO", MB_ICONERROR);
                break;
            }
            if (numDosis == auxVac->dosis) {
                MessageBox(NULL, L"Esta persona ya no tiene dosis pendientes. No es necesario calcular.", L"AVISO", MB_ICONERROR);
                break;
            }
            SendMessage(GetDlgItem(hDlg, IDC_DIASDOSIS), WM_GETTEXT, sizeof(diasDosisBuffer) / sizeof(diasDosisBuffer[0]), (LPARAM)diasDosisBuffer);
            diasEntreDosis = _wtoi(diasDosisBuffer);
            if (wcslen(diasDosisBuffer) == 0) {
                MessageBox(NULL, L"Por favor llene el campo 'Días a esperar' primero.", L"AVISO", MB_ICONERROR);
                break;
            }
            if (diasEntreDosis <= 0) {
                MessageBox(NULL, L"Los días a esperar antes de la siguiente dosis deben ser mayor a 0.", L"AVISO", MB_ICONERROR);
                break;
            }
            SendMessage(GetDlgItem(hDlg, IDC_DIA), WM_GETTEXT, sizeof(dia2Buffer) / sizeof(dia2Buffer[0]), (LPARAM)dia2Buffer);
            dia = _wtoi(dia2Buffer);
            fechaDosis.dia = dia;
            SendMessage(GetDlgItem(hDlg, IDC_MES), WM_GETTEXT, sizeof(mes2Buffer) / sizeof(mes2Buffer[0]), (LPARAM)mes2Buffer);
            mes = _wtoi(mes2Buffer);
            fechaDosis.mes = mes;
            SendMessage(GetDlgItem(hDlg, IDC_ANIO), WM_GETTEXT, sizeof(anio2Buffer) / sizeof(anio2Buffer[0]), (LPARAM)anio2Buffer);
            anio = _wtoi(anio2Buffer);
            fechaDosis.anio = anio;
            if (wcslen(dia2Buffer) == 0 || wcslen(mes2Buffer) == 0 || wcslen(anio2Buffer) == 0) {
                MessageBox(NULL, L"Por favor llene todos los campos de 'Fecha de esta dosis' primero.", L"AVISO", MB_ICONERROR);
                break;
            }
            #pragma endregion
            sumFecha(fechaDosis, diasEntreDosis, sigDosis);
            swprintf_s(dia3Buffer, 30, L"%d", sigDosis.dia);
            SetDlgItemText(hDlg, IDC_DIASIG, dia3Buffer);
            swprintf_s(mes3Buffer, 30, L"%d", sigDosis.mes);
            SetDlgItemText(hDlg, IDC_MESSIG, mes3Buffer);
            swprintf_s(anio3Buffer, 30, L"%d", sigDosis.anio);
            SetDlgItemText(hDlg, IDC_ANIOSIG, anio3Buffer);
        }
        if (LOWORD(wParam) == IDC_SAVE) {
            SendMessage(GetDlgItem(hDlg, IDC_DOSIS), WM_GETTEXT, sizeof(numDosisBuffer) / sizeof(numDosisBuffer[0]), (LPARAM)numDosisBuffer);
            numDosis = _wtoi(numDosisBuffer);
            SendMessage(GetDlgItem(hDlg, IDC_DIA), WM_GETTEXT, sizeof(dia2Buffer) / sizeof(dia2Buffer[0]), (LPARAM)dia2Buffer);
            dia = _wtoi(dia2Buffer);
            fechaDosis.dia = dia;
            SendMessage(GetDlgItem(hDlg, IDC_MES), WM_GETTEXT, sizeof(mes2Buffer) / sizeof(mes2Buffer[0]), (LPARAM)mes2Buffer);
            mes = _wtoi(mes2Buffer);
            fechaDosis.mes = mes;
            SendMessage(GetDlgItem(hDlg, IDC_ANIO), WM_GETTEXT, sizeof(anio2Buffer) / sizeof(anio2Buffer[0]), (LPARAM)anio2Buffer);
            anio = _wtoi(anio2Buffer);
            fechaDosis.anio = anio;
            SendMessage(GetDlgItem(hDlg, IDC_DIASDOSIS), WM_GETTEXT, sizeof(diasDosisBuffer) / sizeof(diasDosisBuffer[0]), (LPARAM)diasDosisBuffer);
            diasEntreDosis = _wtoi(diasDosisBuffer);
            SendMessage(GetDlgItem(hDlg, IDC_DIASIG), WM_GETTEXT, sizeof(dia3Buffer) / sizeof(dia3Buffer[0]), (LPARAM)dia3Buffer);
            dia = _wtoi(dia3Buffer);
            sigDosis.dia = dia;
            SendMessage(GetDlgItem(hDlg, IDC_MESSIG), WM_GETTEXT, sizeof(mes3Buffer) / sizeof(mes3Buffer[0]), (LPARAM)mes3Buffer);
            mes = _wtoi(mes3Buffer);
            sigDosis.mes = mes;
            SendMessage(GetDlgItem(hDlg, IDC_ANIOSIG), WM_GETTEXT, sizeof(anio3Buffer) / sizeof(anio3Buffer[0]), (LPARAM)anio3Buffer);
            anio = _wtoi(anio3Buffer);
            sigDosis.anio = anio;
            SendMessage(GetDlgItem(hDlg, IDC_LOTE), WM_GETTEXT, sizeof(lote) / sizeof(lote[0]), (LPARAM)lote);
            SendMessage(GetDlgItem(hDlg, IDC_CENTRO), WM_GETTEXT, sizeof(centroVacu) / sizeof(centroVacu[0]), (LPARAM)centroVacu);

            if (wcslen(numDosisBuffer) == 0 || wcslen(dia2Buffer) == 0 || wcslen(mes2Buffer) == 0 ||
                wcslen(anio2Buffer) == 0 || wcslen(lote) == 0 || wcslen(centroVacu) == 0) {
                MessageBox(NULL, L"Por favor complete toda la información", L"AVISO", MB_ICONERROR);
                break;
            }
            if (numDosis <= 0) {
                MessageBox(NULL, L"Su dosis actual debe ser mayor a 0.", L"AVISO", MB_ICONERROR);
                break;
            }
            if (numDosis > auxVac->dosis) {
                MessageBox(NULL, L"Su dosis actual no puede ser mayor al que tiene la vacuna que selecciono.", L"AVISO", MB_ICONERROR);
                break;
            }
            // si la persona tiene otra dosis pendiente, no puede registrar el carne hasta haber calculado la proxima fecha
            if ((numDosis < auxVac->dosis) && (wcslen(diasDosisBuffer) == 0 || wcslen(dia3Buffer) == 0 || wcslen(mes3Buffer) == 0
                || wcslen(anio3Buffer) == 0 || diasEntreDosis == 0 || dia == 0 || mes == 0 || anio == 0)) {
                MessageBox(NULL, L"Por favor calcule la fecha de su próxima dosis.", L"AVISO", MB_ICONERROR);
                break;
            }
            // si lo anterior no aplica, significa que esta en su ultima dosis y estos datos los dejamos en 0
            if (diasEntreDosis == 0 || dia == 0 || mes == 0 || anio == 0) {
                diasEntreDosis = 0;
                sigDosis.dia = 0;
                sigDosis.mes = 0;
                sigDosis.anio = 0;
            }

            editCarne(auxCarne);
            MessageBox(NULL, L"¡La edición del carné fue exitosa!", L"AVISO", MB_ICONINFORMATION);
            EndDialog(hDlg, LOWORD(wParam));
            if (!mainOpen) DialogBox(NULL, MAKEINTRESOURCE(IDD_MAIN), hDlg, MainMenu);
        }
        break;
    }
    case WM_CLOSE: {
        DialogBox(NULL, MAKEINTRESOURCE(IDD_CLOSE), hDlg, ExitCheck);
        if (closeWnd) {
            EndDialog(hDlg, LOWORD(wParam));
            if (!mainOpen) PostQuitMessage(0);
        }
        break;
    }
    default:
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK DeleteCarne(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) { // Para eliminar cualquier vacuna con una clave valida
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
    case WM_INITDIALOG: {
        SetDlgItemText(hDlg, IDC_SHOWUSER, nombreComp);
        globalSel = -1;
        if (firstCarne != NULL) {
            listSize = myCarnes.getSize();
            listCount = 0;
            while (listCount < listSize) { // muestra los numeros unicos de carné registrados
                auxCarne = myCarnes.get(listCount);
                swprintf_s(carnetIDBuffer, 30, L"%d", auxCarne->carnetID);
                if (auxCarne->userID == logUser.userID) {
                    SendDlgItemMessage(hDlg, IDC_COMBONUM, CB_ADDSTRING, NULL, (LPARAM)carnetIDBuffer);
                }
                listCount++;
            }
        }
        if (SendDlgItemMessage(hDlg, IDC_COMBONUM, CB_GETCOUNT, NULL, NULL) == 0) {
            MessageBox(NULL, L"No hay ningun carné registrado que eliminar.", L"AVISO", MB_ICONERROR);
            EndDialog(hDlg, LOWORD(wParam));
            if (!mainOpen) DialogBox(NULL, MAKEINTRESOURCE(IDD_MAIN), hDlg, MainMenu);
        }
        return (INT_PTR)TRUE;
    }
    case WM_COMMAND: {
        if (HIWORD(wParam) == CBN_SELENDOK) { // cuando el usuario seleccione una opcion del combo box
            // busca usando busqueda binaria
            // completa
            if (LOWORD(wParam) == IDC_COMBONUM) {
                int sel = SendDlgItemMessage(hDlg, IDC_COMBONUM, CB_GETCURSEL, NULL, NULL);
                SendDlgItemMessage(hDlg, IDC_COMBONUM, CB_GETLBTEXT, sel, (LPARAM)carnetIDBuffer);
                ///////
                carnetID = _wtoi(carnetIDBuffer);
                auxCarne = myCarnes.binarySearch(carnetID);
                ///////
                #pragma region persona get
                auxPer = auxCarne->persona;
                SetDlgItemText(hDlg, IDP_APEPAT, auxPer->apePat);
                SetDlgItemText(hDlg, IDP_APEMAT, auxPer->apeMat);
                SetDlgItemText(hDlg, IDP_NOMBRES, auxPer->nombres);
                SetDlgItemText(hDlg, IDP_CURP, auxPer->curp);
                SetDlgItemText(hDlg, IDP_RFC, auxPer->rfc);
                swprintf_s(diaBuffer, 30, L"%d", auxPer->fechaNac.dia);
                SetDlgItemText(hDlg, IDP_DIA, diaBuffer);
                swprintf_s(mesBuffer, 30, L"%d", auxPer->fechaNac.mes);
                SetDlgItemText(hDlg, IDP_MES, mesBuffer);
                swprintf_s(anioBuffer, 30, L"%d", auxPer->fechaNac.anio);
                SetDlgItemText(hDlg, IDP_ANIO, anioBuffer);
                SetDlgItemText(hDlg, IDP_CIVIL, auxPer->estCivil);
                SetDlgItemText(hDlg, IDP_CALLE, auxPer->calle);
                SetDlgItemText(hDlg, IDP_COLONIA, auxPer->colonia);
                SetDlgItemText(hDlg, IDP_CIUDAD, auxPer->ciudad);
                SetDlgItemText(hDlg, IDP_ESTADO, auxPer->estado);
                SetDlgItemText(hDlg, IDP_COLONIA, auxPer->colonia);
                SetDlgItemText(hDlg, IDP_TEL, auxPer->phone);
                SetDlgItemText(hDlg, IDP_ARCHIVO, auxPer->archivoID);
                fotoBM = (HBITMAP)LoadImage(NULL, auxPer->foto, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
                SendDlgItemMessage(hDlg, IDP_FOTO, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)fotoBM);
                #pragma endregion
                ///////
                #pragma region vacuna get
                auxVac = auxCarne->vacuna;
                SetDlgItemText(hDlg, IDV_TIPO, auxVac->tipo);
                SetDlgItemText(hDlg, IDV_MARCA, auxVac->marca);
                SetDlgItemText(hDlg, IDV_CLAVE2, auxVac->clave);
                swprintf_s(dosisBuffer, 30, L"%d", auxVac->dosis);
                SetDlgItemText(hDlg, IDV_DOSIS, dosisBuffer);
                SetDlgItemText(hDlg, IDV_DESC, auxVac->desc);
                swprintf_s(precioBuffer, 30, L"%.2f", auxVac->precio);
                SetDlgItemText(hDlg, IDV_PRECIO, precioBuffer);
                #pragma endregion
                ///////
                swprintf_s(numDosisBuffer, 30, L"%d", auxCarne->numDosis);
                SetDlgItemText(hDlg, IDC_DOSIS, numDosisBuffer);
                swprintf_s(dia2Buffer, 30, L"%d", auxCarne->fechaDosis.dia);
                SetDlgItemText(hDlg, IDC_DIA, dia2Buffer);
                swprintf_s(mes2Buffer, 30, L"%d", auxCarne->fechaDosis.mes);
                SetDlgItemText(hDlg, IDC_MES, mes2Buffer);
                swprintf_s(anio2Buffer, 30, L"%d", auxCarne->fechaDosis.anio);
                SetDlgItemText(hDlg, IDC_ANIO, anio2Buffer);
                swprintf_s(diasDosisBuffer, 30, L"%d", auxCarne->diasEntreDosis);
                SetDlgItemText(hDlg, IDC_DIASDOSIS, diasDosisBuffer);
                swprintf_s(dia3Buffer, 30, L"%d", auxCarne->sigDosis.dia);
                SetDlgItemText(hDlg, IDC_DIASIG, dia3Buffer);
                swprintf_s(mes3Buffer, 30, L"%d", auxCarne->sigDosis.mes);
                SetDlgItemText(hDlg, IDC_MESSIG, mes3Buffer);
                swprintf_s(anio3Buffer, 30, L"%d", auxCarne->sigDosis.anio);
                SetDlgItemText(hDlg, IDC_ANIOSIG, anio3Buffer);
                swprintf_s(diasDosisBuffer, 30, L"%d", auxCarne->diasEntreDosis);
                SetDlgItemText(hDlg, IDC_LOTE, auxCarne->lote);
                SetDlgItemText(hDlg, IDC_CENTRO, auxCarne->centroVacu);
            }
        }
        if (LOWORD(wParam) == IDC_DELETE) {
            if (globalSel != -1) { // si se hizo busqueda secuencial
                myCarnes.del(globalSel--);
            }
            else if ((globalSel == -1) && auxCarne != NULL){  // si se hizo busqueda binaria
                listSize = myCarnes.getSize();
                listCount = 0;
                foundCarne = false;
                while ((listCount < listSize) && !foundCarne) {
                    auxCarne = myCarnes.get(listCount);
                    if (auxCarne->userID == logUser.userID) {
                        if (auxCarne->carnetID == carnetID) {
                            myCarnes.del(listCount);
                            foundCarne = true;
                        }
                    }
                    listCount++;
                }
            }
            else {
                MessageBox(NULL, L"Por favor seleccione un carné a eliminar.", L"AVISO", MB_ICONERROR);
                break;
            }
            MessageBox(NULL, L"¡La eliminación del carné fue exitosa!", L"AVISO", MB_ICONINFORMATION);
            EndDialog(hDlg, LOWORD(wParam));
            if (!mainOpen) DialogBox(NULL, MAKEINTRESOURCE(IDD_MAIN), hDlg, MainMenu);
        }
        break;
    }
    case WM_CLOSE: {
        DialogBox(NULL, MAKEINTRESOURCE(IDD_CLOSE), hDlg, ExitCheck);
        if (closeWnd) {
            EndDialog(hDlg, LOWORD(wParam));
            if (!mainOpen) PostQuitMessage(0);
        }
        break;
    }
    default:
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK BuscarCarne(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) { // Despliega carnés y demás por persona 
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
    case WM_INITDIALOG: {
        SetDlgItemText(hDlg, IDC_SHOWUSER, nombreComp);
        globalSel = -1;
        if (firstCarne != NULL) {
            chosePerson = false;
            listSize = myCarnes.getSize();
            listCount = 0;
            while (listCount < listSize) { // muestra todas las personas con al menos un carné registrado
                auxCarne = myCarnes.get(listCount);
                swprintf_s(carnetIDBuffer, 30, L"%d", auxCarne->carnetID);
                if (auxCarne->userID == logUser.userID) {
                    setNombreCompPersona(*auxCarne->persona);
                    if (SendDlgItemMessage(hDlg, IDC_COMBOPER, CB_FINDSTRING, -1, (LPARAM)nombreCompPersona) == CB_ERR) {
                        SendDlgItemMessage(hDlg, IDC_COMBOPER, CB_ADDSTRING, NULL, (LPARAM)nombreCompPersona);
                    }
                }
                listCount++;
            }
        }
        if (SendDlgItemMessage(hDlg, IDC_COMBOPER, CB_GETCOUNT, NULL, NULL) == 0) {
            MessageBox(NULL, L"No hay ningun carné registrado.", L"AVISO", MB_ICONERROR);
            EndDialog(hDlg, LOWORD(wParam));
            if (!mainOpen) DialogBox(NULL, MAKEINTRESOURCE(IDD_MAIN), hDlg, MainMenu);
        }
        return (INT_PTR)TRUE;
    }
    case WM_COMMAND: {
        if (HIWORD(wParam) == CBN_SELENDOK) { // cuando el usuario seleccione una opcion de los combo box
            if (LOWORD(wParam) == IDC_COMBOPER) {
                if (!chosePerson) chosePerson = true;
                int sel = SendDlgItemMessage(hDlg, IDC_COMBOPER, CB_GETCURSEL, NULL, NULL);
                SendDlgItemMessage(hDlg, IDC_COMBOPER, CB_GETLBTEXT, sel, (LPARAM)nombreCompCheck);
                SendDlgItemMessage(hDlg, IDC_COMBONUM, CB_RESETCONTENT, 0, 0); // limpia el combobox cada vez
                listSize = myCarnes.getSize();
                listCount = 0;
                while (listCount < listSize) { // muestra solo los carnés registrados con el nombre seleccionado
                    auxCarne = myCarnes.get(listCount);
                    swprintf_s(carnetIDBuffer, 30, L"%d", auxCarne->carnetID);
                    if (auxCarne->userID == logUser.userID) {
                        setNombreCompPersona(*auxCarne->persona);
                        if (wcscmp(nombreCompPersona, nombreCompCheck) == 0) {
                            SendDlgItemMessage(hDlg, IDC_COMBONUM, CB_ADDSTRING, NULL, (LPARAM)carnetIDBuffer);
                        }
                    }
                    listCount++;
                }
            }
            // busca usando busqueda secuencial
            if (LOWORD(wParam) == IDC_COMBONUM) {
                if (chosePerson) {
                    int sel = SendDlgItemMessage(hDlg, IDC_COMBOPER, CB_GETCURSEL, NULL, NULL);
                    SendDlgItemMessage(hDlg, IDC_COMBOPER, CB_GETLBTEXT, sel, (LPARAM)nombreCompCheck);
                    int sel2 = SendDlgItemMessage(hDlg, IDC_COMBONUM, CB_GETCURSEL, NULL, NULL);
                    SendDlgItemMessage(hDlg, IDC_COMBONUM, CB_GETLBTEXT, sel2, (LPARAM)carnetIDBuffer);
                    carnetID = _wtoi(carnetIDBuffer);
                    ///////
                    listSize = myCarnes.getSize();
                    listCount = 0;
                    foundCarne = false;
                    while ((listCount < listSize) && !foundCarne) {
                        auxCarne = myCarnes.get(listCount);
                        if (auxCarne->userID == logUser.userID) {
                            setNombreCompPersona(*auxCarne->persona);
                            if (wcscmp(nombreCompPersona, nombreCompCheck) == 0) {
                                if (wcscmp(nombreCompPersona, nombreCompCheck) == 0) {
                                    if (auxCarne->carnetID == carnetID) { // aparte de buscar el nombre, busca el ID indicado (una persona puede tener mas de 1 carne)
                                        swprintf_s(numDosisBuffer, 30, L"%d", auxCarne->numDosis);
                                        SetDlgItemText(hDlg, IDC_DOSIS, numDosisBuffer);
                                        swprintf_s(dia2Buffer, 30, L"%d", auxCarne->fechaDosis.dia);
                                        SetDlgItemText(hDlg, IDC_DIA, dia2Buffer);
                                        swprintf_s(mes2Buffer, 30, L"%d", auxCarne->fechaDosis.mes);
                                        SetDlgItemText(hDlg, IDC_MES, mes2Buffer);
                                        swprintf_s(anio2Buffer, 30, L"%d", auxCarne->fechaDosis.anio);
                                        SetDlgItemText(hDlg, IDC_ANIO, anio2Buffer);
                                        swprintf_s(diasDosisBuffer, 30, L"%d", auxCarne->diasEntreDosis);
                                        SetDlgItemText(hDlg, IDC_DIASDOSIS, diasDosisBuffer);
                                        swprintf_s(dia3Buffer, 30, L"%d", auxCarne->sigDosis.dia);
                                        SetDlgItemText(hDlg, IDC_DIASIG, dia3Buffer);
                                        swprintf_s(mes3Buffer, 30, L"%d", auxCarne->sigDosis.mes);
                                        SetDlgItemText(hDlg, IDC_MESSIG, mes3Buffer);
                                        swprintf_s(anio3Buffer, 30, L"%d", auxCarne->sigDosis.anio);
                                        SetDlgItemText(hDlg, IDC_ANIOSIG, anio3Buffer);
                                        swprintf_s(diasDosisBuffer, 30, L"%d", auxCarne->diasEntreDosis);
                                        SetDlgItemText(hDlg, IDC_LOTE, auxCarne->lote);
                                        SetDlgItemText(hDlg, IDC_CENTRO, auxCarne->centroVacu);
                                        globalSel = listCount;
                                        foundCarne = true;
                                    }
                                }
                            }
                        }
                        listCount++;
                    }
                    ///////
#pragma region persona get
                    auxPer = auxCarne->persona;
                    SetDlgItemText(hDlg, IDP_APEPAT, auxPer->apePat);
                    SetDlgItemText(hDlg, IDP_APEMAT, auxPer->apeMat);
                    SetDlgItemText(hDlg, IDP_NOMBRES, auxPer->nombres);
                    SetDlgItemText(hDlg, IDP_CURP, auxPer->curp);
                    SetDlgItemText(hDlg, IDP_RFC, auxPer->rfc);
                    swprintf_s(diaBuffer, 30, L"%d", auxPer->fechaNac.dia);
                    SetDlgItemText(hDlg, IDP_DIA, diaBuffer);
                    swprintf_s(mesBuffer, 30, L"%d", auxPer->fechaNac.mes);
                    SetDlgItemText(hDlg, IDP_MES, mesBuffer);
                    swprintf_s(anioBuffer, 30, L"%d", auxPer->fechaNac.anio);
                    SetDlgItemText(hDlg, IDP_ANIO, anioBuffer);
                    SetDlgItemText(hDlg, IDP_CIVIL, auxPer->estCivil);
                    SetDlgItemText(hDlg, IDP_CALLE, auxPer->calle);
                    SetDlgItemText(hDlg, IDP_COLONIA, auxPer->colonia);
                    SetDlgItemText(hDlg, IDP_CIUDAD, auxPer->ciudad);
                    SetDlgItemText(hDlg, IDP_ESTADO, auxPer->estado);
                    SetDlgItemText(hDlg, IDP_COLONIA, auxPer->colonia);
                    SetDlgItemText(hDlg, IDP_TEL, auxPer->phone);
                    SetDlgItemText(hDlg, IDP_ARCHIVO, auxPer->archivoID);
                    fotoBM = (HBITMAP)LoadImage(NULL, auxPer->foto, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
                    SendDlgItemMessage(hDlg, IDP_FOTO, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)fotoBM);
#pragma endregion
                    ///////
#pragma region vacuna get
                    auxVac = auxCarne->vacuna;
                    SetDlgItemText(hDlg, IDV_TIPO, auxVac->tipo);
                    SetDlgItemText(hDlg, IDV_MARCA, auxVac->marca);
                    SetDlgItemText(hDlg, IDV_CLAVE2, auxVac->clave);
                    swprintf_s(dosisBuffer, 30, L"%d", auxVac->dosis);
                    SetDlgItemText(hDlg, IDV_DOSIS, dosisBuffer);
                    SetDlgItemText(hDlg, IDV_DESC, auxVac->desc);
                    swprintf_s(precioBuffer, 30, L"%.2f", auxVac->precio);
                    SetDlgItemText(hDlg, IDV_PRECIO, precioBuffer);
#pragma endregion
                    ///////
                }
            }
        }
        break;
    }
    case WM_CLOSE: {
        DialogBox(NULL, MAKEINTRESOURCE(IDD_CLOSE), hDlg, ExitCheck);
        if (closeWnd) {
            EndDialog(hDlg, LOWORD(wParam));
            if (!mainOpen) PostQuitMessage(0);
        }
        break;
    }
    default:
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK RepNombre(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) { // Genera y despliega un reporte de personas vacunadas
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
    case WM_INITDIALOG: {
        SetDlgItemText(hDlg, IDR_SHOWUSER, nombreComp);

        int n = myCarnes.getSize();
        quickSort(myCarnes, 0, n); // ordena los datos por # de carne de manera ascendiente

        // solamente muestra los carnes con personas que ya recibieron una dosis 
        if (firstCarne != NULL) {
            listSize = myCarnes.getSize();
            listCount = 0;
            while (listCount < listSize) { // muestra los numeros unicos de carné registrados
                auxCarne = myCarnes.get(listCount);
                if (isVaccinated(auxCarne->fechaDosis)) {
                    swprintf_s(carnetIDBuffer, 30, L"%d", auxCarne->carnetID);
                    SendDlgItemMessage(hDlg, IDR_CARNE, LB_ADDSTRING, NULL, (LPARAM)carnetIDBuffer);
                    setFechaCompleta(auxCarne->fechaDosis); // junta la fecha a un formato Dia/Mes/Año dentro de fechaBuffer
                    SendDlgItemMessage(hDlg, IDR_DOSIS, LB_ADDSTRING, NULL, (LPARAM)fechaBuffer);
                    auxPer = auxCarne->persona;
                    SendDlgItemMessage(hDlg, IDR_APEP, LB_ADDSTRING, NULL, (LPARAM)auxPer->apePat);
                    SendDlgItemMessage(hDlg, IDR_APEM, LB_ADDSTRING, NULL, (LPARAM)auxPer->apeMat);
                    SendDlgItemMessage(hDlg, IDR_NOMBRE, LB_ADDSTRING, NULL, (LPARAM)auxPer->nombres);
                    setFechaCompleta(auxPer->fechaNac);
                    SendDlgItemMessage(hDlg, IDR_FECHA, LB_ADDSTRING, NULL, (LPARAM)fechaBuffer);
                    SendDlgItemMessage(hDlg, IDR_CALLE, LB_ADDSTRING, NULL, (LPARAM)auxPer->calle);
                    SendDlgItemMessage(hDlg, IDR_COLONIA, LB_ADDSTRING, NULL, (LPARAM)auxPer->colonia);
                    SendDlgItemMessage(hDlg, IDR_CIUDAD, LB_ADDSTRING, NULL, (LPARAM)auxPer->ciudad);
                    SendDlgItemMessage(hDlg, IDR_ESTADO, LB_ADDSTRING, NULL, (LPARAM)auxPer->estado);
                    SendDlgItemMessage(hDlg, IDR_TEL, LB_ADDSTRING, NULL, (LPARAM)auxPer->phone);
                    SendDlgItemMessage(hDlg, IDR_DOC, LB_ADDSTRING, NULL, (LPARAM)auxPer->archivoID);
                }
                listCount++;
            }
        }
        return (INT_PTR)TRUE;
    }
    case WM_COMMAND: {
        if (LOWORD(wParam) == IDR_EXPORT) {
            saveReport(myCarnes, "reporteNombre.txt");
            MessageBox(NULL, L"¡La creación de 'reporteNombre.txt' fue exitosa!", L"AVISO", MB_ICONINFORMATION);
            EndDialog(hDlg, LOWORD(wParam));
            if (!mainOpen) DialogBox(NULL, MAKEINTRESOURCE(IDD_MAIN), hDlg, MainMenu);
        }
        break;
    }
    case WM_CLOSE: {
        DialogBox(NULL, MAKEINTRESOURCE(IDD_CLOSE), hDlg, ExitCheck);
        if (closeWnd) {
            EndDialog(hDlg, LOWORD(wParam));
            if (!mainOpen) PostQuitMessage(0);
        }
        break;
    }
    default:
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK RepCarne(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) { // Genera y despliega un reporte de personas vacunadas
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
    case WM_INITDIALOG: {
        SetDlgItemText(hDlg, IDR_SHOWUSER, nombreComp);

        int n = myCarnes.getSize();
        heapSort(myCarnes, n); // ordena los datos por # de carne de manera ascendiente

        // solamente muestra los carnes con personas que ya recibieron una o mas dosis 
        if (firstCarne != NULL) {
            listSize = myCarnes.getSize();
            listCount = 0;
            while (listCount < listSize) { // muestra los numeros unicos de carné registrados
                auxCarne = myCarnes.get(listCount);
                if (isVaccinated(auxCarne->fechaDosis)) { // checa que la fecha sea anterior al dia de hoy
                    swprintf_s(carnetIDBuffer, 30, L"%d", auxCarne->carnetID);
                    SendDlgItemMessage(hDlg, IDR_CARNE, LB_ADDSTRING, NULL, (LPARAM)carnetIDBuffer);
                    setFechaCompleta(auxCarne->fechaDosis); // junta la fecha a un formato Dia/Mes/Año dentro de fechaBuffer
                    SendDlgItemMessage(hDlg, IDR_DOSIS, LB_ADDSTRING, NULL, (LPARAM)fechaBuffer);
                    auxPer = auxCarne->persona;
                    SendDlgItemMessage(hDlg, IDR_APEP, LB_ADDSTRING, NULL, (LPARAM)auxPer->apePat);
                    SendDlgItemMessage(hDlg, IDR_APEM, LB_ADDSTRING, NULL, (LPARAM)auxPer->apeMat);
                    SendDlgItemMessage(hDlg, IDR_NOMBRE, LB_ADDSTRING, NULL, (LPARAM)auxPer->nombres);
                    setFechaCompleta(auxPer->fechaNac);
                    SendDlgItemMessage(hDlg, IDR_FECHA, LB_ADDSTRING, NULL, (LPARAM)fechaBuffer);
                    SendDlgItemMessage(hDlg, IDR_CALLE, LB_ADDSTRING, NULL, (LPARAM)auxPer->calle);
                    SendDlgItemMessage(hDlg, IDR_COLONIA, LB_ADDSTRING, NULL, (LPARAM)auxPer->colonia);
                    SendDlgItemMessage(hDlg, IDR_CIUDAD, LB_ADDSTRING, NULL, (LPARAM)auxPer->ciudad);
                    SendDlgItemMessage(hDlg, IDR_ESTADO, LB_ADDSTRING, NULL, (LPARAM)auxPer->estado);
                    SendDlgItemMessage(hDlg, IDR_TEL, LB_ADDSTRING, NULL, (LPARAM)auxPer->phone);
                    SendDlgItemMessage(hDlg, IDR_DOC, LB_ADDSTRING, NULL, (LPARAM)auxPer->archivoID);
                }
                listCount++;
            }
        }
        return (INT_PTR)TRUE;
    }
    case WM_COMMAND: {
        if (LOWORD(wParam) == IDR_EXPORT) {
            saveReport(myCarnes, "reporteID.txt");
            MessageBox(NULL, L"¡La creación de 'reporteID.txt' fue exitosa!", L"AVISO", MB_ICONINFORMATION);
            EndDialog(hDlg, LOWORD(wParam));
            if (!mainOpen) DialogBox(NULL, MAKEINTRESOURCE(IDD_MAIN), hDlg, MainMenu);
        }
        break;
    }
    case WM_CLOSE: {
        DialogBox(NULL, MAKEINTRESOURCE(IDD_CLOSE), hDlg, ExitCheck);
        if (closeWnd) {
            EndDialog(hDlg, LOWORD(wParam));
            if (!mainOpen) PostQuitMessage(0);
        }
        break;
    }
    default:
        break;
    }
    return (INT_PTR)FALSE;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE leg, LPSTR com, int modo) {
    firstUser = auxUser = NULL;
    firstPer = auxPer = NULL;
    firstVac = auxVac = NULL;
    firstCarne = auxCarne = NULL;
    getMaxIDUser();
    loadFromFileUser();
    ///////////////
    DialogBox(hInst, MAKEINTRESOURCE(ID_LOGIN), NULL, LogIn);
    return 0;
}
