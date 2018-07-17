#* This file is part of the MOOSE framework
#* https://www.mooseframework.org
#*
#* All rights reserved, see COPYRIGHT for full restrictions
#* https://github.com/idaholab/moose/blob/master/COPYRIGHT
#*
#* Licensed under LGPL 2.1, please see LICENSE for details
#* https://www.gnu.org/licenses/lgpl-2.1.html

import os, re, math

class CSVDiffer:
    def __init__(self, test_dir, out_files, abs_zero=1e-11, relative_error=5.5e-6, gold_dir='gold',
            custom_columns=[], custom_rel_err=[], custom_abs_zero=[]):
        self.abs_zero = float(abs_zero)
        self.rel_tol = float(relative_error)
        self.custom_columns = custom_columns
        self.custom_rel_err = custom_rel_err
        self.custom_abs_zero = custom_abs_zero
        self.files = []
        self.msg = ''
        self.num_errors = 0

        # read in files
        for out_file in out_files:
            # Check for an easy mistake to make
            if out_file[-2:] == '.e':
                self.msg += 'Diffing ' + out_file + '\n'
                self.msg += 'WARNING: Are you sure you want to use csv diff on a .e file?\n'

            test_filename = os.path.join(test_dir,out_file)
            gold_filename = os.path.join(test_dir, gold_dir, out_file)
            if not os.path.exists(test_filename):
                self.addError(test_filename, 'File does not exist!')
            elif not os.path.exists(gold_filename):
                self.addError(gold_filename, 'Gold file does not exist!')
            else:
                try:
                    f1 = open( test_filename )
                    f2 = open( gold_filename )
                    self.addCSVPair(out_file, f1.read(), f2.read())
                except Exception as e:
                    self.addError(out_file, 'Exception reading files '+str(e.args))


    # add the contents of two files to be diffed
    def addCSVPair(self, fname, text1, text2):
        self.files.append( (fname, text1, text2) )

    def clearDiff(self):
        """Clears the diff message and errors so diff can be called again"""
        self.msg = ''
        self.num_errors = 0

    # diff the files added to the system and return a message of differences
    # This method should only be called once. If it called again you must
    # manually clear messages by calling clearDiff
    def diff(self):

        # Setup data structures for holding customized relative tolerance and absolute
        # zero values and flag for checking variable names
        rel_err_map = {}
        abs_zero_map = {}
        found_column = {}
        for i in range(0, len(self.custom_columns)):
            rel_err_map[self.custom_columns[i]] = float(self.custom_rel_err[i])
            abs_zero_map[self.custom_columns[i]] = float(self.custom_abs_zero[i])
            found_column[self.custom_columns[i]] = False

        for fname, text1, text2 in self.files:
            # use this value to skip the rest of the tests when we've found an error
            # the order of the tests is most general to most specific, so if a general
            # one fails then the more specific ones will probably not only fail, but
            # crash the program because it's looking in a column that doesn't exist
            foundError = False

            table1 = self.convertToTable(fname, text1)
            table2 = self.convertToTable(fname, text2)

            # Make sure header names are the same (also makes sure # cols is the same)
            # This way it reports what column is missing, not just # cols is different
            keys1 = table1.keys()
            keys2 = table2.keys()
            (large,small) = (keys1,keys2)
            if len(keys1) < len(keys2):
                (large,small) = (keys2,keys1)
            for key in large:
                if key not in small:
                    self.addError(fname, "Header '" + key + "' is missing" )
                    foundError = True
            if foundError:
                continue

            # check if custom tolerances used, column name exists in one of
            # the CSV files
            if self.custom_columns:
               for mykey in self.custom_columns:
                   if mykey in small:
                      found_column[mykey] = True

            # now check that each column is the same length
            for key in keys1:
                if len(table1[key]) != len(table2[key]):
                    self.addError(fname, "Columns with header '" + key + "' aren't the same length")
                    foundError = True
                    # assume all columns are the same length, so don't report the other errors
                    break
            if foundError:
                continue

            # now check all the values in the table
            abs_zero = self.abs_zero
            rel_tol   = self.rel_tol
            for key in keys1:
                for val1, val2 in zip( table1[key], table2[key] ):
                    # if customized tolerances specified use them otherwise
                    # use the default
                    if self.custom_columns:
                        try:
                            abs_zero = abs_zero_map[key]
                        except:
                            abs_zero = self.abs_zero
                    if abs(val1) < abs_zero:
                        val1 = 0
                    if abs(val2) < abs_zero:
                        val2 = 0

                    # disallow nan in the gold file
                    if math.isnan(val1):
                        self.addError(fname, "The values in column \"" + key.strip() + "\" contain NaN")

                    # disallow inf in the gold file
                    if math.isinf(val1):
                        self.addError(fname, "The values in column \"" + key.strip() + "\" contain Inf")

                    # if they're both exactly zero (due to the threshold above) then they're equal so pass this test
                    if val1 == 0 and val2 == 0:
                        continue

                    rel_diff = 0
                    if max( abs(val1), abs(val2) ) > 0:
                        rel_diff = abs( ( val1 - val2 ) / max( abs(val1), abs(val2) ) )

                    # if customized tolerances specified use them otherwise
                    # use the default
                    if self.custom_columns:
                        try:
                            rel_tol = rel_err_map[key]
                        except:
                            rel_tol = self.rel_tol
                    if rel_diff > rel_tol:
                        self.addError(fname, "The values in column \"" + key.strip() + "\" don't match\n\trel diff:  " + str(val1) + " ~ " + str(val2) + " = " + str(rel_diff))
                        # assume all other vals in this column are wrong too, so don't report them
                        break

        # Loop over variable names to check if any are missing from all the
        # CSV files being compared
        if self.custom_columns:
           for mykey2 in self.custom_columns:
               if not found_column[mykey2]:
                  self.addError("all CSV files", "Variable '" + mykey2 + "' in custom_columns is missing" )

        return self.msg


    # convert text to a map of column names to column values
    def convertToTable(self, fname, text):
        # ignore newlines
        text = re.sub( r'\n\s*\n', '\n', text).strip()

        # Exceptions occur if you try to parse a .e file
        try:
            lines = text.split('\n')
            headers = lines.pop(0).split(',')
            table = {}
            for header in headers:
                table[header] = []

            for row in lines:
                vals = row.split(',')
                if len(headers) != len(vals):
                    self.addError(fname, "Number of columns ("+str(len(vals))+") not the same as number of column names ("+str(len(headers))+") in row "+repr(row))
                for header, val in zip(headers,vals):
                    try:
                        table[header].append(float(val))
                    except:
                        # ignore strings
                        table[header].append(0)

        except Exception as e:
            self.addError(fname, "Exception parsing file: "+str(e.args))
            return {}

        return table

    # add an error to the message
    # every error is added through here, so it could also output in xml, etc.
    def addError(self, fname, message):
        self.msg += 'In ' + fname + ': ' + message + '\n'
        self.num_errors += 1

    def getNumErrors(self):
        """Return number of errors in diff"""
        return self.num_errors
