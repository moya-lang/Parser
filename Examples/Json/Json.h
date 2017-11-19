#ifndef JsonH
#define JsonH

#include <iostream>
#include <iomanip>

#include "Parser.h"
#include "JsonSyntax.h"
#include "JsonObject.h"
#include "JsonObjectType.h"

class Json
{
    JsonSyntax jsonSyntax;

    void print(std::string &content, Moya::ObjectTree &objectTree)
    {
        std::map<std::size_t, std::string> names;
        names[jsonSyntax.json.getData().id] = "json";
        names[jsonSyntax.object.getData().id] = "object";
        names[jsonSyntax.members.getData().id] = "members";
        names[jsonSyntax.pair.getData().id] = "pair";
        names[jsonSyntax.array.getData().id] = "array";
        names[jsonSyntax.elements.getData().id] = "elements";
        names[jsonSyntax.value.getData().id] = "value";
        names[jsonSyntax.whitespaces.getData().id] = "whitespaces";
        names[jsonSyntax.string.getData().id] = "string";
        names[jsonSyntax.character.getData().id] = "character";
        names[jsonSyntax.hexDigit.getData().id] = "hexDigit";
        names[jsonSyntax.number.getData().id] = "number";
        names[jsonSyntax.digit.getData().id] = "digit";

        for (std::size_t objectId = 0; objectTree.hasNext(); objectId = objectTree.next()) {

            int depth = 0;
            for (std::size_t parent = objectTree->parent; parent != objectTree.nullObject; parent = objectTree[parent].parent)
                depth++;

            std::cout << std::setfill('0') << std::setw(4) << objectId;
            std::cout << ": " << std::string(depth, ' ');

            if (names.find(objectTree->syntaxId) != names.end())
                std::cout << names[objectTree->syntaxId].c_str();
            std::cout << "[" << objectTree->syntaxId << "]";

            std::cout << ", '" << std::string(&content.c_str()[objectTree->from], &content.c_str()[objectTree->to]) << "'";
            std::cout << std::endl;
        }
    }


    public:

        Json(int argumentCount, char **arguments)
        {
            (void) argumentCount;
            (void) arguments;
        }

        int run()
        {
            std::string content = "{\"abc\":123}";
                //"  { \"abc\":{\"abc\":{\"abc\":{\"a\":\"b\",\"a\":\"b\",\"a\":\"b\"}}}, \"ab\\ncd\"  :  [ \"str\", 5, 6 , 555, true , false,null   ]  }  ";
                //"{\"web-app\":{\"servlet\":[{\"servlet-name\":\"cofaxCDS\",\"servlet-class\":\"org.cofax.cds.CDSServlet\",\"init-param\":{\"configGlossary:installationAt\":\"Philadelphia, PA\",\"configGlossary:adminEmail\":\"ksm@pobox.com\",\"configGlossary:poweredBy\":\"Cofax\",\"configGlossary:poweredByIcon\":\"/images/cofax.gif\",\"configGlossary:staticPath\":\"/content/static\",\"templateProcessorClass\":\"org.cofax.WysiwygTemplate\",\"templateLoaderClass\":\"org.cofax.FilesTemplateLoader\",\"templatePath\":\"templates\",\"templateOverridePath\":\"\",\"defaultListTemplate\":\"listTemplate.htm\",\"defaultFileTemplate\":\"articleTemplate.htm\",\"useJSP\":false,\"jspListTemplate\":\"listTemplate.jsp\",\"jspFileTemplate\":\"articleTemplate.jsp\",\"cachePackageTagsTrack\":200,\"cachePackageTagsStore\":200,\"cachePackageTagsRefresh\":60,\"cacheTemplatesTrack\":100,\"cacheTemplatesStore\":50,\"cacheTemplatesRefresh\":15,\"cachePagesTrack\":200,\"cachePagesStore\":100,\"cachePagesRefresh\":10,\"cachePagesDirtyRead\":10,\"searchEngineListTemplate\":\"forSearchEnginesList.htm\",\"searchEngineFileTemplate\":\"forSearchEngines.htm\",\"searchEngineRobotsDb\":\"WEB-INF/robots.db\",\"useDataStore\":true,\"dataStoreClass\":\"org.cofax.SqlDataStore\",\"redirectionClass\":\"org.cofax.SqlRedirection\",\"dataStoreName\":\"cofax\",\"dataStoreDriver\":\"com.microsoft.jdbc.sqlserver.SQLServerDriver\",\"dataStoreUrl\":\"jdbc:microsoft:sqlserver://LOCALHOST:1433;DatabaseName=goon\",\"dataStoreUser\":\"sa\",\"dataStorePassword\":\"dataStoreTestQuery\",\"dataStoreTestQuery\":\"SET NOCOUNT ON;select test='test';\",\"dataStoreLogFile\":\"/usr/local/tomcat/logs/datastore.log\",\"dataStoreInitConns\":10,\"dataStoreMaxConns\":100,\"dataStoreConnUsageLimit\":100,\"dataStoreLogLevel\":\"debug\",\"maxUrlLength\":500}},{\"servlet-name\":\"cofaxEmail\",\"servlet-class\":\"org.cofax.cds.EmailServlet\",\"init-param\":{\"mailHost\":\"mail1\",\"mailHostOverride\":\"mail2\"}},{\"servlet-name\":\"cofaxAdmin\",\"servlet-class\":\"org.cofax.cds.AdminServlet\"},{\"servlet-name\":\"fileServlet\",\"servlet-class\":\"org.cofax.cds.FileServlet\"},{\"servlet-name\":\"cofaxTools\",\"servlet-class\":\"org.cofax.cms.CofaxToolsServlet\",\"init-param\":{\"templatePath\":\"toolstemplates/\",\"log\":1,\"logLocation\":\"/usr/local/tomcat/logs/CofaxTools.log\",\"logMaxSize\":\"\",\"dataLog\":1,\"dataLogLocation\":\"/usr/local/tomcat/logs/dataLog.log\",\"dataLogMaxSize\":\"\",\"removePageCache\":\"/content/admin/remove?cache=pages&id=\",\"removeTemplateCache\":\"/content/admin/remove?cache=templates&id=\",\"fileTransferFolder\":\"/usr/local/tomcat/webapps/content/fileTransferFolder\",\"lookInContext\":1,\"adminGroupID\":4,\"betaServer\":true}}],\"servlet-mapping\":{\"cofaxCDS\":\"/\",\"cofaxEmail\":\"/cofaxutil/aemail/*\",\"cofaxAdmin\":\"/admin/*\",\"fileServlet\":\"/static/*\",\"cofaxTools\":\"/tools/*\"},\"taglib\":{\"taglib-uri\":\"cofax.tld\",\"taglib-location\":\"/WEB-INF/tlds/cofax.tld\"}}}";
                //"{\"glossary\":{\"title\":\"example glossary\",\"GlossDiv\":{\"title\":\"S\",\"GlossList\":{\"GlossEntry\":{\"ID\":\"SGML\",\"SortAs\":\"SGML\",\"GlossTerm\":\"Standard Generalized Markup Language\",\"Acronym\":\"SGML\",\"Abbrev\":\"ISO 8879:1986\",\"GlossDef\":{\"para\":\"A meta - markup language, used to create markup languages such as DocBook.\",\"GlossSeeAlso\":[\"GML\",\"XML\"]},\"GlossSee\":\"markup\"}}}}}";

            Moya::ObjectTree objectTree;
            Moya::StringSequencer stringSequencer(content);
            Moya::Parser parser(jsonSyntax.json, stringSequencer, objectTree);

            if (parser.parse())
                print(content, objectTree);
            else
                std::cout << "Error: " << parser.getErrorString() << std::endl;

            return 0;
        }
};

#endif
