#include <revolution/os.h>
#include <revolution/gx.h>
#include <revolution/vi.h>
#include <revolution/dvd.h>
#include <revolution/sc.h>

extern u16 OSSetFontEncode(u16);

static void (*FatalFunc)(void) = NULL;

const char* const __DVDErrorMessageDefault[] = {
    "\n\n\nエラーが発生しました。\n"
    "\n"
    "イジェクトボタンを押してディスクを取り出してか\n"
    "ら、本体の電源をOFFにして、本体の取扱説明書の\n"
    "指示に従ってください。",

    "\n\n\nAn error has occurred.\n"
    "Press the Eject Button, remove the\n"
    "Game Disc, and turn the power off.\n"
    "Please read the Wii Operations Manual\n"
    "for more information.",

    "\n\n\nEin Fehler ist aufgetreten.\n"
    "Dr・ke den Ausgabeknopf, entnimm die\n"
    "Disc und schalte die Wii-Konsole aus.\n"
    "Bitte lies die Wii-Bedienungsanleitung,\n"
    "um weitere Informationen zu erhalten.",

    "\n\n\nUne erreur est survenue.\n"
    "Appuyez sur le bouton EJECT, retirez\n"
    "le disque et 騁eignez la console.\n"
    "Veuillez vous r馭駻er au mode d'emploi\n"
    "Wii pour plus de d騁ails.",

    "\n\n\nSe ha producido un error.\n"
    "Pulsa el Bot EJECT, extrae el disco y\n"
    "apaga la consola. Consulta el manual de\n"
    "instrucciones de la consola Wii para\n"
    "obtener m疽 informaci.",

    "\n\n\nSi ・verificato un errore.\n"
    "Premi il pulsante EJECT, estrai il disco\n"
    "e spegni la console. Per maggiori\n"
    "informazioni, consulta il manuale di\n"
    "istruzioni della console Wii.",

    "\n\n\nEr is een fout opgetreden.\n"
    "Druk op de EJECT-knop, verwijder de\n"
    "disk en zet het Wii-systeem uit. Lees\n"
    "de handleiding voor meer informatie."
};

const char* const __DVDErrorMessageEurope[] = {
    "\n\n\nエラーが発生しました。\n"
    "\n"
    "イジェクトボタンを押してディスクを取り出してか\n"
    "ら、本体の電源をOFFにして、本体の取扱説明書の\n"
    "指示に従ってください。",

    "\n\n\nAn error has occurred.\n"
    "Press the EJECT Button, remove the\n"
    "Game Disc, and turn the power off.\n"
    "Please read the Wii Operations Manual\n"
    "for more information.",

    "\n\n\nEin Fehler ist aufgetreten.\n"
    "Dr・ke den Ausgabeknopf, entnimm die\n"
    "Disc und schalte die Wii-Konsole aus.\n"
    "Bitte lies die Wii-Bedienungsanleitung,\n"
    "um weitere Informationen zu erhalten.",

    "\n\n\nUne erreur est survenue.\n"
    "Appuyez sur le bouton EJECT, retirez\n"
    "le disque et 騁eignez la console.\n"
    "Veuillez vous r馭駻er au mode d'emploi\n"
    "Wii pour plus de d騁ails.",

    "\n\n\nSe ha producido un error.\n"
    "Pulsa el Bot EJECT, extrae el disco y\n"
    "apaga la consola. Consulta el manual de\n"
    "instrucciones de la consola Wii para\n"
    "obtener m疽 informaci.",

    "\n\n\nSi ・verificato un errore.\n"
    "Premi il pulsante EJECT, estrai il disco\n"
    "e spegni la console. Per maggiori\n"
    "informazioni, consulta il manuale di\n"
    "istruzioni della console Wii.",

    "\n\n\nEr is een fout opgetreden.\n"
    "Druk op de EJECT-knop, verwijder de\n"
    "disk en zet het Wii-systeem uit. Lees\n"
    "de handleiding voor meer informatie."
};

const char* const __DVDErrorMessage104 [] = {
    "\n"
    "\n"
    "エラーコード１０４。\n"
    "エラーが発生しました。\n"
    "\n"
    "イジェクトボタンを押してディスクを取り出してか\n"
    "ら、本体の電源をOFFにして、本体の取扱説明書の\n"
    "指示に従ってください。",

    "\n"
    "\n"
    "Error #104,\n"
    "An error has occurred.\n"
    "Press the EJECT Button, remove the\n"
    "Game Disc, and turn the power off.\n"
    "Please read the Wii Operations Manual\n"
    "for more information."
};

void __DVDShowFatalMessage(void) {
    const char* message;
    const char* const* messageList;
    GXColor bg = { 0, 0, 0, 0 };
    GXColor fg = { 255, 255, 255, 0 };

    if (SCGetLanguage() == 0) {
        OSSetFontEncode(1);
    }
    else {
        OSSetFontEncode(0);
    }

    switch (SCGetProductGameRegion()) {
        default:
            messageList = __DVDErrorMessage104;
            break;
        case 2:
            messageList = __DVDErrorMessageEurope;
            break;
        case 4:
        case 5:
            messageList = __DVDErrorMessageDefault;
            break;
    }

    if (SCGetLanguage() > 6) {
        message = messageList[1];
    }
    else {
        message = messageList[SCGetLanguage()];
    }

    OSFatal(fg, bg, message);
}

BOOL DVDSetAutoFatalMessaging(BOOL enable) {
    BOOL enabled, prev;
    enabled = OSDisableInterrupts();
    prev = FatalFunc ? TRUE : FALSE;
    FatalFunc = enable ? __DVDShowFatalMessage : NULL;
    OSRestoreInterrupts(enabled);
    return prev;
}

BOOL __DVDGetAutoFatalMessaging(void) {
    return FatalFunc ? TRUE : FALSE;
}

void __DVDPrintFatalMessage(void) {
    if (FatalFunc) {
        FatalFunc();
    }
}