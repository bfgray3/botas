FROM python:3.12

ENV PYTHONUNBUFFERED 1

RUN : \
  && apt-get update \
  && DEBIAN_FRONTEND=noninteractive apt-get install --yes --no-install-recommends python3-dev time build-essential \
  && apt-get clean \
  && rm -rf /var/lib/apt/lists/* \
  && :

ENV PATH=/venv/bin:$PATH

RUN : \
  && python -m venv /venv \
  && pip --no-cache-dir --disable-pip-version-check install --upgrade numpy pip scipy wheel \
  && :

WORKDIR /usr/src/botas

ENTRYPOINT ["./compare.sh"]
