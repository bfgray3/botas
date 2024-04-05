FROM python:3.12

RUN : \
  && apt-get update \
  && DEBIAN_FRONTEND=noninteractive apt-get install --yes --no-install-recommends time \
  && apt-get clean \
  && rm -rf /var/lib/apt/lists/* \
  && :

ENV PATH=/venv/bin:$PATH

RUN : \
  && python -m venv /venv \
  && pip --no-cache-dir install numpy scipy \
  && :

WORKDIR botas

ENTRYPOINT ["./compare.sh"]
