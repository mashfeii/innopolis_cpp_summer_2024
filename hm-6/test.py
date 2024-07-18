#! /usr/bin/env python3

import cjson


def main():
    json_str = '{"world": "value", "hello": 10}'
    cjson_doc = cjson.loads(json_str)
    print(cjson_doc)


if __name__ == "__main__":
    main()
