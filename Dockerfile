FROM python:3.12

RUN : \
  && apt-get update \
  && DEBIAN_FRONTEND=noninteractive apt-get install --yes --no-install-recommends time \
  && apt-get clean \
  && rm -rf /var/lib/apt/lists/* \
  && :

WORKDIR /usr/src/botas

ENV PATH=/venv/bin:$PATH PYTHONUNBUFFERED=1

RUN : \
  && python3 -m venv /venv \
  && pip --no-cache-dir --disable-pip-version-check install --upgrade numpy scipy \
  && :


ENTRYPOINT ["./compare.sh"]
