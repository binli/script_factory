#include "DianPing-query.h"
#include <unity/scopes/Annotation.h>
#include <unity/scopes/CategorisedResult.h>
#include <unity/scopes/CategoryRenderer.h>
#include <unity/scopes/QueryBase.h>
#include <unity/scopes/SearchReply.h>

using namespace unity::scopes;

//Create a URI to be used in the web API query
const QString APPKEY = "593963090";
const QString SIGN = "709C12486A4982627D029AB472EDFE2B417A8E22";
//http://api.dianping.com/v1/deal/find_deals?appkey=593963090&sign=709C12486A4982627D029AB472EDFE2B417A8E22&city=%E5%8C%97%E4%BA%AC&region=%E6%B5%B7%E6%B7%80%E5%8C%BA&category=%E7%BE%8E%E9%A3%9F&limit=3&page=1
const QString BASE_URI = "http://api.dianping.com/v1/deal/find_deals?";

DianpingQuery::DianpingQuery(std::string const& query) :
    query_(query)
{
}

DianpingQuery::~DianpingQuery()
{
}

void DianpingQuery::cancelled()
{
}

void DianpingQuery::run(unity::scopes::SearchReplyProxy const& reply)
{
    CategoryRenderer rdr;
    auto cat = reply->register_category("cat1", "Category 1", "", rdr);
    CategorisedResult res(cat);
    res.set_uri("uri");
    res.set_title("scope-A: result 1 for query \"" + query_ + "\"");
    res.set_art("icon");
    res.set_dnd_uri("dnd_uri");
    reply->push(res);

    CannedQuery q("scope-A", query_, "");
    Annotation annotation(Annotation::Type::Link);
    annotation.add_link("More...", q);
    reply->register_annotation(annotation);

}
