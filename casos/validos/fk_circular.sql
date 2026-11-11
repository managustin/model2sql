CREATE TABLE Categoria (
    id INTEGER PRIMARY KEY,
    elemento_id INTEGER
);

CREATE TABLE Elemento (
    id INTEGER PRIMARY KEY,
    categoria_id INTEGER,
    FOREIGN KEY (categoria_id) REFERENCES Categoria(id)
);

ALTER TABLE Categoria ADD FOREIGN KEY (elemento_id) REFERENCES Elemento(id);
