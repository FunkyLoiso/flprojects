#include "scriptformcontroller.h"

#include "../static.h"

void onSendMagic()
{
    static const byte magic = 0x22;
    Static::ledCubeConnection.Write(&magic, 1);
}

void onSendFrame(byte frame[64])
{
    static bool firstPart = true;

    if(firstPart)
    {
        Static::ledCubeConnection.Write(frame, 32);
    }
    else
    {
        Static::ledCubeConnection.Write(frame+32, 32);
    }
    firstPart = !firstPart;
}

ScriptFormController::ScriptFormController()
{
}

void ScriptFormController::service(HttpRequest& request, HttpResponse& response) {

    QString script = "";
    if (request.getParameter("action")=="show")
    {
        script = request.getParameter("script");
//        response.write("<html><body>");
//        response.write("Script = ");
//        response.write(script.toLatin1());
//        response.write("</body></html>",true);

        static byte frame[64];
        memset(frame, 0, 64*sizeof(byte));
        QStringList coords = script.split('.');
        int x = coords[0].toInt(), y = coords[1].toInt(), z = coords[2].toInt();
        frame[8*z+x] = 1<<y;

//        static bool begin = true;
//        if(begin)
//        {
//            onSendMagic();
//        }
//        onSendFrame(frame);
//        begin = !begin;
        onSendMagic();
        onSendFrame(frame);
        Sleep(5);
        onSendFrame(frame);
    }

    response.setHeader("Content-Type", "text/html; charset=ISO-8859-1");

    response.write("<html><body>");
    response.write("<form method=\"post\">");
    response.write("  <input type=\"hidden\" name=\"action\" value=\"show\">");
    QString tmp = QString("  Script: <input type=\"text\" name=\"script\" value=\"%1\"><br>").arg(script);
    response.write(tmp.toLatin1());
    response.write("  <input type=\"submit\">");
    response.write("</form>");
    response.write("</body></html>",true);

//    else
//    {
//        response.write("<html><body>");
//        response.write("<form method=\"post\">");
//        response.write("  <input type=\"hidden\" name=\"action\" value=\"show\">");
//        response.write("  Script: <input type=\"text\" name=\"script\"><br>");
//        response.write("  <input type=\"submit\">");
//        response.write("</form>");
//        response.write("</body></html>",true);
//    }
}


