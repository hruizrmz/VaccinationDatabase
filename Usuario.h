#ifndef Usuario_h
#define Usuario_h
class Usuario {
protected:
	int userID = 0;
	wchar_t user[150]; // unico
	wchar_t pwd[150];
	wchar_t apePat[150];
	wchar_t apeMat[150];
	wchar_t nombres[150];
	wchar_t nombreComp[300];
public:
	Usuario();
	void setUsuario(wchar_t[150], wchar_t[150], wchar_t[150], wchar_t[150], wchar_t[150]);
	wchar_t* getUser();
	wchar_t* getPwd();
	wchar_t* getApePat();
	wchar_t* getApeMat();
	wchar_t* getNombres();
	wchar_t* getNombreComp();
};
Usuario::Usuario() {
	wcscpy_s(user, L"");
	wcscpy_s(pwd, L"");
	wcscpy_s(apePat, L"");
	wcscpy_s(apeMat, L"");
	wcscpy_s(nombres, L"");
	wcscpy_s(nombreComp, L"");
}
void Usuario::setUsuario(wchar_t user[150], wchar_t pwd[150], wchar_t apePat[150], wchar_t apeMat[150], wchar_t nombres[150]) {
	wcscpy_s(this->user, user);
	wcscpy_s(this->pwd, pwd);
	wcscpy_s(this->apePat, apePat);
	wcscpy_s(this->apeMat, apeMat);
	wcscpy_s(this->nombres, nombres);
	wcscpy_s(nombreComp, L"");
}
wchar_t* Usuario::getUser() {
	return user;
}
wchar_t* Usuario::getPwd() {
	return pwd;
}
wchar_t* Usuario::getApePat() {
	return apePat;
}
wchar_t* Usuario::getApeMat() {
	return apeMat;
}
wchar_t* Usuario::getNombres() {
	return nombres;
}
wchar_t* Usuario::getNombreComp() {
	wcscpy_s(nombreComp, nombres);
	wcscat_s(nombreComp,L" ");
	wcscat_s(nombreComp, apeMat);
	wcscat_s(nombreComp, L" ");
	wcscat_s(nombreComp, apePat);
	return nombreComp;
}
#endif // !Usuario_h

