#!/usr/bin/env python
# -*- coding: utf-8; -*-

import sys

from wsgiref.simple_server import make_server


class WebApplication(object):
    def __init__(self, value):
        self.value = value

    def __call__(self, environ, start_response):
        status = '200 OK'
        headers = [('Content-type', 'text/plain')]
        start_response(status, headers)
        return [str(self.value)]


if __name__ == '__main__':
    try:
        val = sys.argv[1]
    except IndexError:
        val = 8888

    app = WebApplication(val)
    httpd = make_server('', 8000, app)

    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        pass
