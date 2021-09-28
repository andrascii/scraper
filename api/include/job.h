#pragma once

namespace api {

struct Job {
  bool enabled{};
  bool parse_anchors{};
  bool parse_queries{};
  bool get_links_from_sitemap_only{};
  std::string url;
  std::vector<std::string> entry_points;
  std::vector<std::string> ignore_masks;
  std::vector<std::string> match_masks;
  std::string sitemap_url;
  uint64_t update_frequency;
};

}