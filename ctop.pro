TEMPLATE = subdirs

CONFIG += ordered
SUBDIRS = model \
          view \
          app

app.depends = model
app.depends = view
