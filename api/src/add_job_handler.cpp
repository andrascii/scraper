#include "add_job_handler.h"

namespace api {

AddJobHandler::AddJobHandler(std::shared_ptr<PgConnectionPool> pool)
  : pool_{std::move(pool)} {}

IHttpHandler::ExpectedResponse AddJobHandler::Handle(RequestType&& request) noexcept {
  const auto wrapper = pool_->Take();
  (void)request;
  return common::Unexpected<std::error_code>{std::error_code{}};
  // url string
  // update_frequency number
  // parse_queries bool
  // parse_anchors bool
  // ignore_masks string[]
  // match_masks string[]
  // sitemap_url string
  // get_links_from_sitemap_only bool
  // entry_points string[]
  // pagination_operations string[][]
  // operations string[][]
}

}