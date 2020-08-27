
def registerMesser():
   
    

    """
    Registers a new Messer node type using the nb utility class.
    """
    import sys
    print sys.path
    from Katana import Nodes3DAPI,NodegraphAPI
    from Katana import FnAttribute
    import NodegraphAPI.Constants.ApplyWhenOptions as ApplyWhenOptions
    import NodegraphAPI.Constants.ApplyWhereOptions as ApplyWhereOptions
    import NodegraphAPI.Constants.ExecutionModeOptions as ExecutionModeOptions
    from Katana import FnGeolibServices


    def _DoInputRequests(interface, inputBehavior, inputPorts, graphState):
        interface.setExplicitInputRequestsEnabled(True)
        if inputBehavior == 'only valid':
            state = interface.SKIP
        else:
            state = interface.NO_OP
        for inputPort in inputPorts:
            interface.addInputRequest(inputPort.getName(), graphState, invalidInputBehavior = state)

    def buildMesserOpChain(node, interface):
        """
        Defines the callback function used to define the Ops chain for the
        node type being registered.

        @type node: C{Nodes3DAPI.nb.Messer}
        @type interface: C{Nodes3DAPI.nb.BuildChainInterface}
        @param node: The node for which to define the Ops chain
        @param interface: The interface providing the functions needed to set
            up the Ops chain for the given node.
        """
        # Get the current frame time
        # Set the minimum number of input ports
        interface.setMinRequiredInputs(1)



        graphState = interface.getGraphState()
        frameTime = graphState.getTime()

        argsGb = FnAttribute.GroupBuilder()
        argsGb.set('system', graphState.getOpSystemArgs())

  
        argsGb.set('inputIndex', FnAttribute.FloatAttribute(1))
        # Parse the CEL parameter
        celParam = node.getParameter('CEL')
        if celParam:
            argsGb.set('CEL', celParam.getValue(frameTime))

        userParam = node.getParameter('user')
        if userParam:
            userAttr = NodegraphAPI.BuildAttrFromGroupParameter(userParam, graphState)
            if userAttr is not None:
                argsGb.set('user', userAttr)

        # Parse the script parameter
        scriptParam = node.getParameter('script')
        if scriptParam:
            argsGb.set('script', scriptParam.getValue(frameTime))
        
        # xform
        transformParam = node.getParameter('transform')
        if transformParam:
            transformAttr = NodegraphAPI.BuildAttrFromGroupParameter(transformParam, graphState)
            if transformAttr is not None:
                argsGb.set('transform', transformAttr)

       

        opArgs = argsGb.build()
        if opArgs is not None:
            argsGb.deepUpdate(opArgs)
        



        opType = 'CartesianScript'
        #executionMode = node.getParameter('executionMode').getValue(frameTime)
        #if executionMode == ExecutionModeOptions.Immediate:
            #gb = FnAttribute.GroupBuilder()
            #gb.set('system', graphState.getOpSystemArgs())
            #if opArgs is not None:
            #    gb.deepUpdate(opArgs)
            #opArgs = gb.build()
            #asb = FnGeolibServices.OpArgsBuilders.AttributeSet()
            #asb.setCEL(FnAttribute.StringAttribute(node.getParameter('CEL').getValue(frameTime)))
            #asb.addSubOp(opType, opArgs)
            #interface.appendOp('AttributeSet', asb.build())
            #_DoInputRequests(interface, node.getParameter('inputBehavior').getValue(frameTime), node.getInputPorts(), graphState)
        # Add the explicit input request for our input port with the modified graph state
        # Add a variable to the local graph state
        #interface.setExplicitInputRequestsEnabled(True)
        #interface.addInputRequest('i0', graphState)
        #interface.addInputRequest('i1', graphState)
      

         # Add the Messer Op to the Ops chain
        interface.appendOp('CartesianScript', argsGb.build())



    # Create a nb to register the new type
    nb = Nodes3DAPI.NodeTypeBuilder('CartesianScript')
    

    print dir(nb)
    # Add an input port
    nb.setInputPortNames(('i0','i1',))
    nb.setOutputPortNames(('out',))

    # Build the node's parameters
    gb = FnAttribute.GroupBuilder()
    gb.set('CEL', FnAttribute.StringAttribute(''))
    gb.set('script', FnAttribute.StringAttribute('print(Time)'))
    gb.set('executionMode', FnAttribute.StringAttribute('immediate'))
    gb.set('inputBehavior', FnAttribute.StringAttribute('by index'))
    nb.addTransformParameters(gb)
    nb.addMakeInteractiveParameter(gb)
    nb.addTimingParameters(gb)
    #gb.set('transform.rotate', FnAttribute.DoubleAttribute([0,0,0]))
    #gb.set('transform.scale', FnAttribute.DoubleAttribute([0,0,0]))
    # Set the parameters template
    nb.setParametersTemplateAttr(gb.build())
    # Set parameter hints
    nb.setHintsForParameter('CartesianScript', {'widget':'opScriptNode'})
    nb.setHintsForParameter('CEL', {'widget':'cel'})
    nb.setHintsForParameter('script', {'widget':'scriptEditor',
                                                    'highlighter': 'Lua',
                                                    'supportsNonmodalExternalEditing': 'True',
                                                    'resistLabelResize': 'True',
                                                    'externalEditorSuffix': '.lua',
                                                    'mono': 'True'})
    nb.setHintsForParameter('executionMode', {'widget':'popup','options': [ ExecutionModeOptions.Immediate,ExecutionModeOptions.Deferred] })
    nb.setHintsForParameter('inputBehavior', {'widget':'popup','options': [ 'by index','only valid'] })
    # Set the callback responsible to build the Ops chain
    nb.setBuildOpChainFnc(buildMesserOpChain)
    # Build the new node type
    nb.build()

# Register the node
registerMesser()

