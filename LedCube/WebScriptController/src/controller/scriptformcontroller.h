#ifndef SCRIPTFORMCONTROLLER_H
#define SCRIPTFORMCONTROLLER_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"

class ScriptFormController : public HttpRequestHandler {
    Q_OBJECT
    Q_DISABLE_COPY(ScriptFormController);
public:

    /** Constructor */
    ScriptFormController();

    /** Generates the response */
    void service(HttpRequest& request, HttpResponse& response);
};

#endif // SCRIPTFORMCONTROLLER_H
