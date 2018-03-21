import re,math,numpy

def main(channel='DF'):
    # Regex
    pattern = re.compile(r'[-+]?\d*\.\d+|\d+')
    # Read
    input_file  = open('ST2L_VV_SR%s_MORIOND17_PDF.txt'%channel,'r')
    uncertainty             = 0.
    counter                 = 0 
    nominal_transfer_factor = 0.
    # For double check
    arr = [] 
    # Read the data
    for line in input_file:
        result          = pattern.findall(line)
        variation       = int(result[0])
        transfer_factor = float(result[9])*math.pow(10,-1*int(result[10])) 
        if variation==7:
            nominal_transfer_factor = transfer_factor
        elif variation<111:
            arr.append(transfer_factor)
            if nominal_transfer_factor>0:
                counter     = counter + 1
                uncertainty = uncertainty + math.pow(transfer_factor-nominal_transfer_factor,2)
            else:
                print 'Mistake...' 
    # Find standard deviation https://twiki.cern.ch/twiki/bin/view/AtlasProtected/PdfRecommendations#Standard_deviation
    uncertainty = math.sqrt((1/float(counter))*uncertainty)
    print 'TF is      %e +/- %e (%e%%) w/ a total variation of %i'%(nominal_transfer_factor,uncertainty,uncertainty/nominal_transfer_factor*100,counter)
    # Mean Std
    arr = numpy.array(arr)
    print 'From Numpy %e +/- %e (%e%%)'%(numpy.mean(arr),numpy.std(arr),numpy.std(arr)/numpy.mean(arr)*100)

if 'main' in __name__:
    print '\nSame-Flavor\n'
    main('SF')
    print '\nDifferent-Flavor\n'
    main('DF')
