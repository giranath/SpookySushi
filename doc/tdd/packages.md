# Packages
A package is a data-structure which is responsible to store
assets and describe dependencies between them.

## Format
This section will describe the format of a package on disk

### Header
Each package contains an header.
<table>
    <thead>
        <tr>
            <td>0</td>
            <td>1</td>
            <td>2</td>
            <td>3</td>
            <td>4</td>
            <td>5</td>
            <td>6</td>
            <td>7</td>
        </tr>
    </thead>
    <tbody>
    <tr>
        <td colspan="4">Magic</td>
        <td colspan="2">Flags</td>
        <td>Version</td>
        <td>Reserved</td>
    </tr>
    </tbody>
</table>

| Field    | Description                     | Size |
|----------|---------------------------------|------|
| Magic    | A constant to identify the file | 4    |
| Flags    | A bitflag with options          | 2    |
| Version  | Identify the package's version  | 1    |
| Reserved | Unused byte                     | 1    |

#### Magic
The magic number for a package is SAPK: Sushi Asset Package

#### Flags

| Bit | Description      |
|-----|------------------|
| 0   | Is little endian |
| 1   | Reserved         |
| 2   | Reserved         |
| 3   | Reserved         |
| 4   | Reserved         |
| 5   | Reserved         |
| 6   | Reserved         |
| 7   | Reserved         |
| 8   | Reserved         |
| 9   | Reserved         |
| 10  | Reserved         |
| 11  | Reserved         |
| 12  | Reserved         |
| 13  | Reserved         |
| 14  | Reserved         |
| 15  | Reserved         |

#### Version
| Value | Description      |
|-------|------------------|
| 0     | Version 1.0      |

### Entry table
Just after the header, there is an entry table.
The entry table list all the stored packages and give the offset
to read an entry

#### Table header
<table>
    <thead>
        <tr>
            <td>0</td>
            <td>1</td>
            <td>2</td>
            <td>3</td>
            <td>4</td>
            <td>5</td>
            <td>6</td>
            <td>7</td>
        </tr>
    </thead>
    <tbody>
    <tr>
        <td colspan="2">Count</td>
        <td colspan="2">Data Offset</td>
        <td colspan="4">Reserved</td>
    </tr>
    </tbody>
</table>

| Field       | Description                     | Size |
|-------------|---------------------------------|------|
| Count       | The number of entry in table    | 2    |
| Data Offset | The offset at which starts data | 2    |
| Reserved    | Unused byte                     | 4    |

#### Entry row
<table>
    <thead>
        <tr>
            <td>0</td>
            <td>1</td>
            <td>2</td>
            <td>3</td>
            <td>4</td>
            <td>5</td>
            <td>6</td>
            <td>7</td>
            <td>8</td>
            <td>9</td>
            <td>10</td>
            <td>11</td>
            <td>12</td>
            <td>13</td>
            <td>14</td>
            <td>15</td>
            <td>16</td>
            <td>17</td>
            <td>18</td>
            <td>19</td>
            <td>20</td>
            <td>21</td>
            <td>22</td>
            <td>23</td>
            <td>24</td>
            <td>25</td>
            <td>26</td>
            <td>27</td>
            <td>28</td>
            <td>29</td>
            <td>30</td>
            <td>31</td>
        </tr>
    </thead>
    <tbody>
    <tr>
        <td colspan="2">ID</td>
        <td colspan="2">Type</td>
        <td colspan="8">Data Offset</td>
        <td colspan="8">Size</td>
        <td colspan="4">Checksum</td>
        <td colspan="8">Reserved</td>
    </tr>
    </tbody>
</table>

| Field       | Description                                     | Size |
|-------------|-------------------------------------------------|------|
| ID          | A package scoped identifier                     | 2    |
| Type        | The type of the stored asset                    | 2    |
| Data Offset | The byte offset where the asset is stored       | 8    |
| Size        | The size of the stored asset in bytes           | 8    |
| Checksum    | A checksum to verify the integrity of the asset | 4    |
| Reserved    | Unused byte                                     | 8    |
