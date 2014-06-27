#include "DianPing-query.h"
#include <unity/scopes/Annotation.h>
#include <unity/scopes/CategorisedResult.h>
#include <unity/scopes/CategoryRenderer.h>
#include <unity/scopes/QueryBase.h>
#include <unity/scopes/SearchReply.h>

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QCoreApplication>

using namespace unity::scopes;

//Create a JSON string to be used tro create a category renderer - uses grid layout
std::string CR_GRID = R"(
    {
        "schema-version" : 1,
        "template" : {
            "category-layout" : "grid",
            "card-size": "small"
        },
        "components" : {
            "title" : "title",
            "art" : {
                "field": "art",
                "aspect-ratio": 1.6,
                "fill-mode": "fit"
            }
        }
    }
)";

//Create a JSON string to be used tro create a category renderer - uses carousel layout
std::string CR_CAROUSEL = R"(
    {
        "schema-version" : 1,
        "template" : {
            "category-layout" : "carousel",
            "card-size": "small"
        },
        "components" : {
            "title" : "title",
            "art" : {
                "field": "art",
                "aspect-ratio": 1.6,
                "fill-mode": "fit"
            }
        }
    }
)";

//Create a URI to be used in the web API query
const QString APPKEY = "593963090";
const QString SIGN = "709C12486A4982627D029AB472EDFE2B417A8E22";
//"http://api.dianping.com/v1/business/find_businesses?appkey=593963090&sign=92FA0BE8777A55C8783D1329D4E59E025ABFCC8E&category=%E7%BE%8E%E9%A3%9F&city=%E5%8C%97%E4%BA%AC&sort=2&limit=20&platform=%1";
//const QString BASE_URI = "http://api.dianping.com/v1/business/find_businesses?appkey=%1&sing=%2&category=%3&city=%4&sort=2&limit=10&platform=2";
const QString BASE_URI = "http://api.dianping.com/v1/business/find_businesses?appkey=593963090&sign=33AE5BA58440E994DBA6358B0E5763C3B85BDFC7&category=%E7%BE%8E%E9%A3%9F&city=%1&sort=2&limit=20&platform=2";

DianpingQuery::DianpingQuery(std::string const& query) :
    query_(query)
{
}

DianpingQuery::~DianpingQuery()
{
}

void DianpingQuery::cancelled()
{
    // TODO: it would be nice to stop QEventLoop that's used in run() here
}

void DianpingQuery::run(unity::scopes::SearchReplyProxy const& reply)
{
    CategoryRenderer rdrGrid(CR_GRID);
    CategoryRenderer rdrCarousel(CR_CAROUSEL);

    auto topGrid = reply->register_category("dianpinggrid", "DianPingGrid", "", rdrGrid);
    auto topCar = reply->register_category("dianpingcarousel", "DianPingCarousel", "", rdrCarousel);

    QEventLoop loop;

    QNetworkAccessManager manager;

    QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));

    QObject::connect(&manager, &QNetworkAccessManager::finished,
            [reply, topCar, topGrid, this](QNetworkReply *msg){
                QByteArray data = msg->readAll();
                QString json = data;
                qDebug() << "Data:" << json;
                QJsonParseError err;
                QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8(), &err);

                if (err.error != QJsonParseError::NoError) {
                    qCritical() << "Failed to parse server data: " << err.errorString();
                } else {
                    // Find the "payload" array of results
                    QJsonObject obj = doc.object();
                    QJsonArray results = obj["businesses"].toArray();

                    // for each result
                    const std::shared_ptr<const Category> * top;

                    bool grid = false;
                    //loop through results of our web query with each result called 'result'
                    for(const auto &result : results) {
                        if ( grid ) {
                          top = &topGrid;
                          grid = false;
                        } else {
                          grid = true;
                          top = &topCar;
                        }
                        //create our categorized result using our pointer, which is either to out
                        //grid or our carousel Category
                        CategorisedResult catres((*top));

                        //treat result as Q JSON
                        QJsonObject resJ = result.toObject();

                        // load up vars with from result
                        auto name = resJ["name"].toString();
                        auto business_uri = resJ["business_url"].toString();
                        auto s_photo_uri = resJ["s_photo_url"].toString();
                        auto photo_uri = resJ["photo_url"].toString();
                        auto rating_s_img_uri = resJ["rating_s_img_url"].toString();

                        //set our CateogroisedResult object with out searchresults values 
                        catres.set_uri(business_uri.toStdString());
                        catres.set_dnd_uri(business_uri.toStdString());
                        catres.set_title(name.toStdString());
                        catres.set_art(photo_uri.toStdString());

                        //push the categorized result to the client
                        if (!reply->push(catres)) {
                            break; // false from push() means search waas cancelled
                        }
                    }
                }
            }
            );

    // The query is the search string and was passed to this Query object's constructor by the client
    // Empty search string yields no results with openclipart API.
    // Let's set the query string to "北京" if empty so the scope displays results at first launch
    if ( query_.empty() ) {
      query_ = "%E5%8C%97%E4%BA%AC";
    }
    QString queryUri = BASE_URI.arg(query_.c_str());
    qDebug() << "Uri:" << queryUri ;
    manager.get(QNetworkRequest(QUrl(queryUri)));
    loop.exec();
}
