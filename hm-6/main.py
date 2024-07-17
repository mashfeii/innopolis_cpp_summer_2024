#!/usr/bin/env python3

import cjson
import ujson
import json


def main():
    json_str = '{"hello":10, "world":"value", "something":10}'

    json_doc = json.loads(json_str)  # default Python library
    cjson_doc = cjson.loads(json_str)  # C API module
    ujson_doc = ujson.loads(json_str)  # PIP library
    print("standard=", json_doc, "\nc_api=", cjson_doc, sep='')
    assert json_doc == cjson_doc


if __name__ == "__main__":
    main()
