#include "DianPing-scope.h"
#include "DianPing-query.h"
#include "DianPing-preview.h"
#include <unity-scopes.h>

using namespace unity::scopes;

int DianpingScope::start(std::string const&, unity::scopes::RegistryProxy const&)
{
    return VERSION;
}

void DianpingScope::stop()
{
    delete app;
}

void DianpingScope::run()
{
    // an instance of QApplication is needed to make Qt happy
    int zero = 0;
    app = new QCoreApplication(zero, nullptr);
}

SearchQueryBase::UPtr DianpingScope::search(unity::scopes::CannedQuery const &q,
                                            unity::scopes::SearchMetadata const&)
{
    unity::scopes::SearchQueryBase::UPtr query(new DianpingQuery(q.query_string()));
    return query;
}

PreviewQueryBase::UPtr DianpingScope::preview(Result const& result, ActionMetadata const& /*metadata*/) {
    unity::scopes::PreviewQueryBase::UPtr preview(new DianpingPreview(result.uri()));
    return preview;
}

#define EXPORT __attribute__ ((visibility ("default")))

extern "C"
{

EXPORT
unity::scopes::ScopeBase*
// cppcheck-suppress unusedFunction
UNITY_SCOPE_CREATE_FUNCTION()
{
    return new DianpingScope();
}

EXPORT
void
// cppcheck-suppress unusedFunction
UNITY_SCOPE_DESTROY_FUNCTION(unity::scopes::ScopeBase* scope_base)
{
    delete scope_base;
}

}
