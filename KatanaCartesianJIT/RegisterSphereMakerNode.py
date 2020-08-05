
def registerSphereMaker():
    """
    Registers a new SphereMaker node type using the NodeTypeBuilder utility
    class.
    """

    from Katana import Nodes3DAPI
    from Katana import FnAttribute

    def buildSphereMakerOpChain(node, interface):
        """
        Defines the callback function used to create the Ops chain for the
        node type being registered.

        @type node: C{Nodes3DAPI.NodeTypeBuilder.SphereMaker}
        @type interface: C{Nodes3DAPI.NodeTypeBuilder.BuildChainInterface}
        @param node: The node for which to define the Ops chain
        @param interface: The interface providing the functions needed to set
            up the Ops chain for the given node.
        """
        # Get the current frame time
        frameTime = interface.getGraphState().getTime()

        # Set the minimum number of input ports
        interface.setMinRequiredInputs(0)

        argsGb = FnAttribute.GroupBuilder()

        # Parse node parameters
        locationParam = node.getParameter('location')
        numberOfSpheresParam = node.getParameter('numberOfSpheres')
        if locationParam:
            location = locationParam.getValue(frameTime)

            # The base location is encoded using nested group attributes
            # defining a hierarchy where the elements in the location paths
            # are interleaved with group attributes named 'c' (for child).
            # The last element will contain a group attribute, named 'a',
            # which in turn will hold an attribute defining the number of
            # spheres to be generated.
            # See the Op source code for more details
            locationPaths = location[1:].split('/')[1:]
            attrsHierarchy = 'c.' + '.c.'.join(locationPaths)
            argsGb.set(attrsHierarchy + '.a.numberOfSpheres',
                FnAttribute.IntAttribute(
                    numberOfSpheresParam.getValue(frameTime)))

        # Add the SphereMaker Op to the Ops chain
        interface.appendOp('SphereMaker', argsGb.build())


    # Create a NodeTypeBuilder to register the new type
    nodeTypeBuilder = Nodes3DAPI.NodeTypeBuilder('SphereMaker')

    # Build the node's parameters
    gb = FnAttribute.GroupBuilder()
    gb.set('location',
           FnAttribute.StringAttribute('/root/world/geo/SphereMaker'))
    gb.set('numberOfSpheres', FnAttribute.IntAttribute(20))

    # Set the parameters template
    nodeTypeBuilder.setParametersTemplateAttr(gb.build())

    # Set parameter hints
    nodeTypeBuilder.setHintsForParameter('location',
                                         {'widget':'scenegraphLocation'})
    nodeTypeBuilder.setHintsForParameter('numberOfSpheres', {'int':True})

    # Set the callback responsible to build the Ops chain
    nodeTypeBuilder.setBuildOpChainFnc(buildSphereMakerOpChain)

    # Build the new node type
    nodeTypeBuilder.build()

# Register the node
registerSphereMaker()
