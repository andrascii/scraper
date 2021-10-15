# scraper

## core

`core` is the static library containing all the functionality.

This approach gives us an ability to cover it by tests:
1. we can link this library into the resulting production binary
2. and we can do the same for tests binary

## common

`common` it's the static library like `core` but it contains the common api that can be used in all projects.
The functionality that has no direct connection with the `core` business logic of application.

## TODO
